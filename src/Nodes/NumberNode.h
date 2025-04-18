#pragma once
#include "Nodes/Node.h"

class NumberNode : public Node
{
public:
    NumberNode(ax::NodeEditor::NodeId id);
    std::shared_ptr<Node> Clone() final {return std::make_shared<NumberNode>(*this);};
    void DrawImpl() final;
    void Update() final;
    void SpecialConstructFromJSON(const nlohmann::json& json) final;
    void SpecialSerialze(nlohmann::json& json) final;
    std::string GetNodeTypeName() final;
private:
    int integer = 0;
    double floatingPoint = 0.0;
    bool isFloating;
};