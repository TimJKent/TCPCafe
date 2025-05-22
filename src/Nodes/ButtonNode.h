#pragma once
#include "Nodes/ClonableNode.h"
#include <Nodes/ClonableNode.h>

class ButtonNode : public ClonableNode<ButtonNode>
{
public:
    ButtonNode();
    void DrawImpl() final;
    std::string GetNodeTypeName() final;
};