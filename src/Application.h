#pragma once
#include <asio.hpp>
#include "Window/Window.h"
#include "TCP/Client/TCPClient.h"
#include "TCP/Server/TCPServer.h"
#include "Widgets/SendMessageWidget.h"
#include "imgui_node_editor.h"
#include "Nodes/Node.h"

namespace ed = ax::NodeEditor;

class Application
{
public:
enum MENU_NAME
{
    TCP_CLIENT, TCP_SERVER, NODE_EDITOR
    
};
public:
    Application();
    int Run();
private:
    void DrawMainMenu();
    void DrawTCPClientWindow();
    void DrawTCPServerWindow();
    void DrawNodeEditor();
    void BeginMainPanel();
    void EndMainPanel();
    void SendMessageFromServer(const std::string& message);
    void SendMessageFromClient(const std::string& message);
    int GetNextId();
    void SpawnInputActionNode();

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
    ed::EditorContext* m_Context = nullptr;
    std::vector<std::shared_ptr<Node>> nodes;
    int m_NextId = 1;
};