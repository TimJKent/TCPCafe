#pragma once
#include "Nodes/Node.h"

class ButtonNode : public Node
{
public:
    ButtonNode(ax::NodeEditor::NodeId id);
    std::shared_ptr<Node> Clone() final {return std::make_shared<ButtonNode>(*this);}
    void DrawImpl() final;
    std::string GetNodeTypeName() final;
};