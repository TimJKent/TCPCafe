#pragma once
#include "Nodes/ClonableNode.h"
#include <Nodes/ClonableNode.h>

class AddNode : public ClonableNode<AddNode>
{
public:
    AddNode(ax::NodeEditor::NodeId id);
    void DrawImpl() final;
    void Update() final;
    std::string GetNodeTypeName() final;
};