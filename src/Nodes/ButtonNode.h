#pragma once
#include "Nodes/ClonableNode.h"
#include <Nodes/ClonableNode.h>

class ButtonNode : public ClonableNode<ButtonNode>
{
public:
    ButtonNode(ax::NodeEditor::NodeId id);
    void DrawImpl() final;
    std::string GetNodeTypeName() final;
};