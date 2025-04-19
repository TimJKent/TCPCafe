#pragma once
#include "Nodes/ClonableNode.h"
#include "TCP/Server/TCPServer.h"
#include "Nodes/ClonableNode.h"

class TCPServerNode : public ClonableNode<TCPServerNode>
{
public:
    TCPServerNode(ax::NodeEditor::NodeId id, std::shared_ptr<TCPServer> tcpServer);
    void DrawImpl() final;
    void Update() final;
    void Send();
    std::string GetNodeTypeName() final;
private:
    std::shared_ptr<TCPServer> tcpServer;
    std::string message;
};