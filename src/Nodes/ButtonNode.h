#pragma once
#include "Nodes/Node.h"
#include "Nodes/Pin.h"

class ButtonNode : public Node
{
public:
    ButtonNode();
    void Draw() final;
    std::vector<std::shared_ptr<Pin>> GetPins() final;
private:
    std::shared_ptr<Pin> outputPin;
};