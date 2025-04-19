#pragma once
#include "Nodes/ClonableNode.h"
#include <Nodes/ClonableNode.h>

class ConcatNode : public ClonableNode<ConcatNode>
{
public:
    ConcatNode(ax::NodeEditor::NodeId id);
    void DrawImpl() final;
    void Update() final;
    std::string GetNodeTypeName() final;
};