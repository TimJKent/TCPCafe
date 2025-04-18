#pragma once
#include "Nodes/Node.h"

class ToggleNode : public Node
{
public:
    ToggleNode(ax::NodeEditor::NodeId id);
    std::shared_ptr<Node> Clone() final {return std::make_shared<ToggleNode>(*this);};
    void DrawImpl() final;
    void Update() final;
    void SpecialConstructFromJSON(const nlohmann::json& json) final;
    void SpecialSerialze(nlohmann::json& json) final;
    std::string GetNodeTypeName() final;
private:
    bool output = false;
};