#pragma once

#include <iostream>
#include <asio.hpp>

using asio::ip::tcp;

class TCPClient
{
public:
    TCPClient(asio::io_context& ioContext);
    void OnReadData(std::error_code ec, std::size_t len);
    void Connect(const std::string& ipAddres, uint32_t port);
    void Disconnect();
    void SendMessage(const std::string& message);
    bool IsConnected();
    std::string ConsumeRXData();
private:
    asio::io_context& ioContext;
    std::unique_ptr<tcp::socket> socket;
    std::vector<char> buffer;
    std::string rxData;
};