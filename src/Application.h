#pragma once
#include <asio.hpp>
#include "Window/Window.h"
#include "TCP/Client/TCPClient.h"
#include "TCP/Server/TCPServer.h"
#include "Widgets/SendMessageWidget.h"


class Application
{
public:
enum MENU_NAME
{
    TCP_CLIENT, TCP_SERVER
};
public:
    Application();
    int Run();
private:
    void DrawMainMenu();
    void DrawTCPClientWindow();
    void DrawTCPServerWindow();
    void BeginMainPanel();
    void EndMainPanel();
    void SendMessageFromServer(const std::string& message);
    void SendMessageFromClient(const std::string& message);

private:
    Window window;
    std::string clientSend;
    std::string clientReceive;
    std::string serverSend;
    std::string serverReceive;

    std::unique_ptr<SendMessageWidget> tcpClientSendMessage1;
    std::unique_ptr<SendMessageWidget> tcpClientSendMessage2;
    std::unique_ptr<SendMessageWidget> tcpClientSendMessage3;

    std::unique_ptr<SendMessageWidget> tcpServerSendMessage1;
    std::unique_ptr<SendMessageWidget> tcpServerSendMessage2;
    std::unique_ptr<SendMessageWidget> tcpServerSendMessage3;

    MENU_NAME activeMenu;
private:
    asio::io_context ioContext;
    TCPClient tcpClient;
    TCPServer tcpServer;
};