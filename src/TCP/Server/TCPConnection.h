#pragma once
#include <asio.hpp>

class TCPConnection
{
public:
    static std::shared_ptr<TCPConnection> Create(asio::io_context& io_context, std::string& rxData);
    TCPConnection(asio::io_context& ioContext, std::string& rxData);
    void OnReadData(std::error_code ec, std::size_t len);
    void Start();
    asio::ip::tcp::socket& GetSocket();
    void Close();
    void SendMessage(const std::string& message);
private:
    asio::ip::tcp::socket socket;
    std::string& rxData;
    std::vector<char> buffer;
    std::string message;
};