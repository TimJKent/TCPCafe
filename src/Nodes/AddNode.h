#pragma once
#include "Nodes/ClonableNode.h"

class AddNode : public ClonableNode<AddNode>
{
public:
    AddNode();
    void DrawImpl() final;
    void Update() final;
    std::string GetNodeTypeName() final;
};