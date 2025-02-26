#pragma once
#include <stdint.h>
#include <memory>
#include <TCP/Server/TCPConnection.h>


class TCPServer
{
public:
    TCPServer(asio::io_context& ioContext);
    void Listen(uint16_t listeningPort);
    void StopListen();
    bool IsListening();
    std::string ConsumeRXData();
    void SendMessage(const std::string& message);
private:
    void StartAccept();
    void HandleAccept(std::shared_ptr<TCPConnection> newConnection, const asio::error_code& error);
private:
    std::unique_ptr<asio::ip::tcp::acceptor> acceptor;
    asio::io_context& ioContext;
    bool isListening = false;
    std::vector<std::shared_ptr<TCPConnection>> connections;
    std::string rxData;
};