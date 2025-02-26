#pragma once
#include "Window/Window.h"
#include "TCP/Client/TCPClient.h"
#include "TCP/Server/TCPServer.h"
#include <asio.hpp>

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

private:
    Window window;
    std::string clientSend;
    std::string clientReceive;
    std::string serverSend;
    std::string serverReceive;
    std::string clientMessage1;
    std::string clientMessage2;
    std::string clientMessage3;
    std::string serverMessage1;
    std::string serverMessage2;
    std::string serverMessage3;
    MENU_NAME activeMenu;
private:
    asio::io_context ioContext;
    TCPClient tcpClient;
    TCPServer tcpServer;
};