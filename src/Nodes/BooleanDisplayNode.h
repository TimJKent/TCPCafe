#pragma once
#include "Nodes/ClonableNode.h"
#include <Nodes/ClonableNode.h>

class BooleanDisplayNode : public ClonableNode<BooleanDisplayNode>
{
public:
    BooleanDisplayNode();
    void DrawImpl() final;
    std::string GetNodeTypeName() final;
};