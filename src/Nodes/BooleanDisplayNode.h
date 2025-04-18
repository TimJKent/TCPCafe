#pragma once
#include "Nodes/Node.h"

class BooleanDisplayNode : public Node
{
public:
    BooleanDisplayNode(ax::NodeEditor::NodeId id);
    std::shared_ptr<Node> Clone() final {return std::make_shared<BooleanDisplayNode>(*this);};
    void DrawImpl() final;
    std::string GetNodeTypeName() final;
};