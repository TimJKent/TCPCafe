#pragma once
#include "Nodes/ClonableNode.h"

class SubtractNode : public ClonableNode<SubtractNode>
{
public:
    SubtractNode();
    void DrawImpl() final;
    void Update() final;
    std::string GetNodeTypeName() final;
};