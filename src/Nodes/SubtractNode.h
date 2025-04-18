#pragma once
#include "Nodes/Node.h"

class SubtractNode : public Node
{
public:
    SubtractNode(ax::NodeEditor::NodeId id);
    std::shared_ptr<Node> Clone() final {return std::make_shared<SubtractNode>(*this);};
    void DrawImpl() final;
    void Update() final;
    std::string GetNodeTypeName() final;
};