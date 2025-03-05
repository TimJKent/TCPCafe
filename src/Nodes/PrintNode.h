#pragma once
#include "Nodes/Node.h"
#include "Nodes/Pin.h"

class PrintNode : public Node
{
public:
    PrintNode();
    void Draw() final;
private:
    Pin inputPin;
    Pin outputPin;
};