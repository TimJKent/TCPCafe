#pragma once
#include "Nodes/Node.h"
#include "Nodes/Pin.h"
#include "TCP/Server/TCPServer.h"

class TCPServerNode : public Node
{
public:
TCPServerNode(std::shared_ptr<TCPServer> tcpServer);
    void Draw() final;
    void Send();
    std::vector<std::shared_ptr<Pin>> GetPins() final;
private:
    std::shared_ptr<Pin> triggerSendPin;
    std::shared_ptr<Pin> stringPin;
    std::shared_ptr<TCPServer> tcpServer;
    std::string message;
};