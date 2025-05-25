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
    TCP_CLIENT, TCP_SERVER, NODE_EDITOR, MODULES
    
};
public:
    Application();
    int Run();
private:
    void DrawTitleBar();
    void DrawTCPClientWindow();
    void DrawTCPServerWindow();
    void DrawNodeEditor();
    void DrawModuleWindow();
    void BeginMainPanel();
    void EndMainPanel();
    void SendMessageFromServer(const std::string& message);
    void SendMessageFromClient(const std::string& message);
    void UpdateWindowTitle();
    void StartNewFile();
    void StartOpenFile();
    void SaveFile();
    void SaveFileAs();
    void OpenFile(const std::string& path);
    void AppWideShortcuts();
    std::shared_ptr<Node> DrawNodeSpawnList();
private:
    Window window;
    std::string clientSend;
    std::string clientReceive;
    std::string serverSend;
    std::string serverReceive;
    ImFont* font_SegoeUIRegularTiny;
    ImFont* font_SegoeUIRegularSmall;
    ImFont* font_SegoeUIRegularMedium;
    ImFont* font_SegoeUIRegularLarge;
    std::unique_ptr<SendMessageWidget> tcpClientSendMessage1;
    std::unique_ptr<SendMessageWidget> tcpClientSendMessage2;
    std::unique_ptr<SendMessageWidget> tcpClientSendMessage3;
    std::unique_ptr<SendMessageWidget> tcpServerSendMessage1;
    std::unique_ptr<SendMessageWidget> tcpServerSendMessage2;
    std::unique_ptr<SendMessageWidget> tcpServerSendMessage3;
    FileManager::Session session;

    MENU_NAME activeMenu = MENU_NAME::NODE_EDITOR;
private:
    asio::io_context ioContext;
    std::shared_ptr<TCPClient> tcpClient;
    std::shared_ptr<TCPServer> tcpServer;
    NodeManager nodeManager;
};