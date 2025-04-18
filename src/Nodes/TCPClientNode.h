#pragma once
#include "Nodes/Node.h"
#include "TCP/Client/TCPClient.h"

class TCPClientNode : public Node
{
public:
    TCPClientNode(ax::NodeEditor::NodeId id, std::shared_ptr<TCPClient> tcpClient);
    std::shared_ptr<Node> Clone() final {return std::make_shared<TCPClientNode>(*this);};
    void DrawImpl() final;
    void Update() final;
    void Send();
    std::string GetNodeTypeName() final;
private:
    std::shared_ptr<TCPClient> tcpClient;
    std::string message;
};