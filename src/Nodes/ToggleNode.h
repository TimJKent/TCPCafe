#pragma once
#include "Nodes/ClonableNode.h"

class ToggleNode : public ClonableNode<ToggleNode>
{
public:
    ToggleNode();
    void DrawImpl() final;
    void Update() final;
    void SpecialConstructFromJSON(const nlohmann::json& json) final;
    void SpecialSerialze(nlohmann::json& json) final;
    std::string GetNodeTypeName() final;
private:
    bool output = false;
};