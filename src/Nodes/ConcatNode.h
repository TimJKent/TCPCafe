#pragma once
#include "Nodes/Node.h"
#include "Nodes/Pin.h"

class ConcatNode : public Node
{
public:
    ConcatNode();
    void Draw() final;
    void Update() final;
    std::vector<std::shared_ptr<Pin>> GetPins() final;
    void AddInputPin();
    void RemoveInputPin();
private:
    std::vector<std::shared_ptr<Pin>> inputPins;
    std::shared_ptr<Pin> outputPin;
};