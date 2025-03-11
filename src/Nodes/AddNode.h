#pragma once
#include "Nodes/Node.h"
#include "Nodes/Pin.h"

class AddNode : public Node
{
public:
    AddNode();
    void Draw() final;
    void Update() final;
    std::vector<std::shared_ptr<Pin>> GetPins() final;
private:
    std::shared_ptr<Pin> inputPin1;
    std::shared_ptr<Pin> inputPin2;
    std::shared_ptr<Pin> outputPin;
};