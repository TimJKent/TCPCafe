#pragma once
#include "Nodes/Node.h"
#include "Nodes/Pin.h"

class StringNode : public Node
{
public:
    StringNode();
    void Draw() final;
    std::vector<std::shared_ptr<Pin>> GetPins() final;
private:
    std::shared_ptr<Pin> outputPin;
    std::string string;
};