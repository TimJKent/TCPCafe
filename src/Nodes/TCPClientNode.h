#pragma once
#include "Nodes/Node.h"
#include "Nodes/Pin.h"
#include "TCP/Client/TCPClient.h"

class TCPClientNode : public Node
{
public:
    TCPClientNode(ax::NodeEditor::NodeId id, std::shared_ptr<TCPClient> tcpClient);
    TCPClientNode(TCPClientNode& copy);
    std::shared_ptr<Node> Clone() final {return std::make_shared<TCPClientNode>(*this);};
    void Draw() final;
    void Send();
    std::vector<std::shared_ptr<Pin>> GetPins() final;
    void ConstructFromJSON(const nlohmann::json& json) final;
    std::string GetNodeTypeName() final;
private:
    std::shared_ptr<Pin> triggerSendPin;
    std::shared_ptr<Pin> stringPin;
    std::shared_ptr<TCPClient> tcpClient;
    std::string message;
};