#pragma once
#include "Nodes/Node.h"
#include "Nodes/Pin.h"

class ConcatNode : public Node
{
public:
    ConcatNode();
    void Draw() final;
    std::vector<std::shared_ptr<Pin>> GetPins() final;
private:
    std::shared_ptr<Pin> stringAPin;
    std::shared_ptr<Pin> stringBPin;
    std::shared_ptr<Pin> outputPin;
};