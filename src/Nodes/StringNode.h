#pragma once
#include "Nodes/Node.h"

class StringNode : public Node
{
public:
    StringNode(ax::NodeEditor::NodeId id);
    std::shared_ptr<Node> Clone() final {return std::make_shared<StringNode>(*this);};
    void DrawImpl() final;
    void Update() final;
    void SpecialConstructFromJSON(const nlohmann::json& json) final;
    void SpecialSerialze(nlohmann::json& json) final;
    std::string GetNodeTypeName() final;
private:
    std::string string;
};