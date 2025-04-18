#pragma once
#include "Nodes/Node.h"

class ConcatNode : public Node
{
public:
    ConcatNode(ax::NodeEditor::NodeId id);
    std::shared_ptr<Node> Clone() final {return std::make_shared<ConcatNode>(*this);}
    void DrawImpl() final;
    void Update() final;
    std::string GetNodeTypeName() final;
};