#pragma once
#include "Nodes/ClonableNode.h"

class PrintNode : public ClonableNode<PrintNode>
{
public:
    PrintNode();
    void DrawImpl() final;
    void Update() final;
    void Print();
    std::string GetNodeTypeName() final;
private:
    std::string message;
    bool risingEdge = true;
};