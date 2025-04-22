#pragma once

#include <asio.hpp>
#include "Window/Window.h"
#include "TCP/Client/TCPClient.h"
#include "TCP/Server/TCPServer.h"
#include "Widgets/SendMessageWidget.h"
#include "imgui_node_editor.h"
#include "Nodes/Node.h"
#include "Nodes/NodeManager.h"
#include "Window/FileManager.h"
#include <utility>

namespace ed = ax::NodeEditor;

class Application
{
public:
enum MENU_NAME
{
    TCP_CLIENT, TCP_SERVER, NODE_EDITOR,
    
};
public:
    Application();
    int Run();
private:
    void DrawTitleBar();
    void DrawTCPClientWindow();
    void DrawTCPServerWindow();
    void DrawNodeEditor();
    void BeginMainPanel();
    void EndMainPanel();
    void SendMessageFromServer(const std::string& message);
    void SendMessageFromClient(const std::string& message);
    void UpdateWindowTitle();
    std::shared_ptr<Node> DrawNodeSpawnList();
private:
    Window window;
    std::string clientSend;
    std::string clientReceive;
    std::string serverSend;
    std::string serverReceive;
    ImFont* font_ConsolasRegular18;
    ImFont* font_ConsolasRegular24;
    ImFont* font_ConsolasRegular36;
    ImFont* font_ConsolasBold18;
    ImFont* font_ConsolasBold24;
    ImFont* font_ConsolasBold36;
    std::unique_ptr<SendMessageWidget> tcpClientSendMessage1;
    std::unique_ptr<SendMessageWidget> tcpClientSendMessage2;
    std::unique_ptr<SendMessageWidget> tcpClientSendMessage3;
    std::unique_ptr<SendMessageWidget> tcpServerSendMessage1;
    std::unique_ptr<SendMessageWidget> tcpServerSendMessage2;
    std::unique_ptr<SendMessageWidget> tcpServerSendMessage3;
    FileManager::Session session;

    MENU_NAME activeMenu = MENU_NAME::NODE_EDITOR;
private:
    std::string activeFileName = "";
    asio::io_context ioContext;
    std::shared_ptr<TCPClient> tcpClient;
    std::shared_ptr<TCPServer> tcpServer;
    NodeManager nodeManager;
};