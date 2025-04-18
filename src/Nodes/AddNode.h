#pragma once
#include "Nodes/Node.h"

class AddNode : public Node
{
public:
    AddNode(ax::NodeEditor::NodeId id);
    std::shared_ptr<Node> Clone() final {return std::make_shared<AddNode>(*this);};
    void DrawImpl() final;
    void Update() final;
    std::string GetNodeTypeName() final;
};