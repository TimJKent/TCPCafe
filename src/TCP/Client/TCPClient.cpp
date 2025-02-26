#include "TCP/Client/TCPClient.h"
#include <asio.hpp>

using asio::ip::tcp;

    TCPClient::TCPClient(asio::io_context& ioContext)
    : ioContext(ioContext)
    , socket(std::make_unique<tcp::socket>(ioContext))
    , buffer(4096)
    {
        
    }

    void TCPClient::OnReadData(std::error_code ec, std::size_t)
    {
        if(ec)
        {
            if(ec == asio::error::operation_aborted)
            {
                rxData.append("\nConnection Closed\n");
            }else
            {
                rxData.append("\n"+ec.message()+"\n");
            }
            Disconnect();
            return;
        }

        rxData.append(buffer.data());
        
        for(size_t i = 0; i < buffer.size(); i++)
        {
            buffer[i] = '\0';
        }

        socket->async_read_some(asio::buffer(buffer, 4096), [this](std::error_code ec, std::size_t len){OnReadData(ec, len);});
    }

    void TCPClient::Connect(const std::string& ipAddres, uint32_t port)
    {
        try{
            tcp::endpoint endpoint(asio::ip::address::from_string(ipAddres), port);
            asio::error_code error;
            socket.reset(new tcp::socket(ioContext));
            socket->connect(endpoint, error);
            
            if(error)
            {
                rxData.append(error.message() + "\n");
                socket->close();
                return;
            }
            
            socket->async_read_some(asio::buffer(buffer, 4096), [this](std::error_code ec, std::size_t len){OnReadData(ec, len);});
        }
        catch (std::exception& e)
        {
            rxData.append(e.what());
        }
    }

    void TCPClient::Disconnect()
    {
        if(IsConnected())
        {
            socket->cancel();
            socket->shutdown(asio::socket_base::shutdown_type::shutdown_both);
            socket->close();
        }
    }

    void TCPClient::SendMessage(const std::string& message)
    {
        if(IsConnected())
        {
            try
            {
                asio::write(*socket.get(), asio::buffer(message));
            }
            catch(std::exception& e)
            {
                rxData.append(e.what());
                Disconnect();
            }
        }
    }

    bool TCPClient::IsConnected()
    {
        return socket->is_open();
    }

    std::string TCPClient::ConsumeRXData()
    {
        std::string output = rxData;
        rxData.clear();
        return output;
    }