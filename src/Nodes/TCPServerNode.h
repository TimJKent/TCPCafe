#pragma once
#include "Nodes/Node.h"
#include "Nodes/Pin.h"
#include "TCP/Server/TCPServer.h"

class TCPServerNode : public Node
{
public:
    TCPServerNode(ax::NodeEditor::NodeId id, std::shared_ptr<TCPServer> tcpServer);
    TCPServerNode(TCPServerNode& copy);
    std::shared_ptr<Node> Clone() final {return std::make_shared<TCPServerNode>(*this);};
    void Draw() final;
    void Send();
    std::vector<std::shared_ptr<Pin>> GetPins() final;
    void ConstructFromJSON(const nlohmann::json& json) final;
    std::string GetNodeTypeName() final;
private:
    std::shared_ptr<Pin> triggerSendPin;
    std::shared_ptr<Pin> stringPin;
    std::shared_ptr<TCPServer> tcpServer;
    std::string message;
};