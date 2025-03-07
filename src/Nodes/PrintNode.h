#pragma once
#include "Nodes/Node.h"
#include "Nodes/Pin.h"

class PrintNode : public Node
{
public:
    PrintNode();
    void Draw() final;
    void Print();
    std::vector<std::shared_ptr<Pin>> GetPins() final;
private:
    std::shared_ptr<Pin> triggerPin;
    std::shared_ptr<Pin> stringPin;
    std::string message;
};