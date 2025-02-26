#include <TCP/Server/TCPConnection.h>

using asio::ip::tcp;

std::shared_ptr<TCPConnection> TCPConnection::Create(asio::io_context& io_context, std::string& rxData)
{
    return std::make_shared<TCPConnection>(io_context, rxData);
}

TCPConnection::TCPConnection(asio::io_context& ioContext, std::string& rxData)
: socket(ioContext)
, rxData(rxData)
, buffer(4096)
{
}

void TCPConnection::OnReadData(std::error_code ec, std::size_t)
{
    if(ec)
    {
        std::string errorMessage = ec == asio::error::operation_aborted ? "\nStopped Listening\n" : "\n" + ec.message()+"\n";
        
        rxData.append(errorMessage);
        Close();
        return;
    }

    rxData.append(buffer.data());
    
    for(size_t i = 0; i< buffer.size(); i++)
    {
        buffer[i] = '\0';
    }

    socket.async_read_some(asio::buffer(buffer, 4096), [this](std::error_code ec, std::size_t len){OnReadData(ec, len);});
}

void TCPConnection::Start()
{
    socket.async_read_some(asio::buffer(buffer, 4096), [this](std::error_code ec, std::size_t len){OnReadData(ec, len);});
}

tcp::socket& TCPConnection::GetSocket()
{
    return socket;
}

void TCPConnection::Close()
{
    if(socket.is_open())
    {
        socket.shutdown(asio::socket_base::shutdown_type::shutdown_both);
        socket.close();
    }
}

void TCPConnection::SendMessage(const std::string& message)
{
    if(socket.is_open())
    {
        try
        {
            asio::write(socket, asio::buffer(message));
        }
        catch(std::exception& e)
        {
            rxData.append(e.what());
        }
    }
}