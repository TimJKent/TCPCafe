#pragma once
#include "Nodes/Node.h"
#include "Nodes/Pin.h"

class IntNode : public Node
{
public:
    IntNode();
    void Draw() final;
    std::vector<std::shared_ptr<Pin>> GetPins() final;
private:
    std::shared_ptr<Pin> outputPin;
    int integer = 0;
};