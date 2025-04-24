#pragma once
#include "Nodes/ClonableNode.h"
#include "TCP/Client/TCPClient.h"

class TCPClientNode : public ClonableNode<TCPClientNode>
{
public:
    TCPClientNode(ax::NodeEditor::NodeId id, std::shared_ptr<TCPClient> tcpClient);
    void DrawImpl() final;
    void Update() final;
    void Send();
    std::string GetNodeTypeName() final;
private:
    std::shared_ptr<TCPClient> tcpClient;
    std::string message;
    bool risingEdge = true;
};