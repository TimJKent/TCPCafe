#pragma once
#include "Nodes/Node.h"

class PrintNode : public Node
{
public:
    PrintNode(ax::NodeEditor::NodeId id);
    std::shared_ptr<Node> Clone() final {return std::make_shared<PrintNode>(*this);};
    void DrawImpl() final;
    void Update() final;
    void Print();
    std::string GetNodeTypeName() final;
private:
    std::string message;
    bool risingEdge = true;
};