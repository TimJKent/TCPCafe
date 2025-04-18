#pragma once
#include "Nodes/Node.h"
#include "TCP/Server/TCPServer.h"

class TCPServerNode : public Node
{
public:
    TCPServerNode(ax::NodeEditor::NodeId id, std::shared_ptr<TCPServer> tcpServer);
    std::shared_ptr<Node> Clone() final {return std::make_shared<TCPServerNode>(*this);};
    void DrawImpl() final;
    void Update() final;
    void Send();
    std::string GetNodeTypeName() final;
private:
    std::shared_ptr<TCPServer> tcpServer;
    std::string message;
};