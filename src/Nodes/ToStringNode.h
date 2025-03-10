#pragma once
#include "Nodes/Node.h"
#include "Nodes/Pin.h"

class ToStringNode : public Node
{
public:
    ToStringNode();
    void Draw() final;
    void Update() final;
    std::vector<std::shared_ptr<Pin>> GetPins() final;
private:
    std::shared_ptr<Pin> inputPin;
    std::shared_ptr<Pin> outputPin;
};