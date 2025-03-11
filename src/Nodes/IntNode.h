#pragma once
#include "Nodes/Node.h"
#include "Nodes/Pin.h"

class IntNode : public Node
{
public:
    IntNode();
    void Draw() final;
    void Update() final;
    std::vector<std::shared_ptr<Pin>> GetPins() final;
private:
    std::shared_ptr<Pin> outputPin;
    std::shared_ptr<Pin> inputPin1;
    std::shared_ptr<Pin> inputPin2;
    int integer = 0;
};