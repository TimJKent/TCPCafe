#pragma once
#include "Nodes/Node.h"
#include "Nodes/Pin.h"
#include "TCP/Client/TCPClient.h"

class ClientSendNode : public Node
{
public:
    ClientSendNode(std::shared_ptr<TCPClient> tcpClient);
    void Draw() final;
    void Send();
    std::vector<std::shared_ptr<Pin>> GetPins() final;
private:
    std::shared_ptr<Pin> triggerSendPin;
    std::shared_ptr<Pin> stringPin;
    std::shared_ptr<TCPClient> tcpClient;
    std::string message;
};