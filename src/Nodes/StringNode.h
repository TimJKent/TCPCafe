#pragma once
#include "Nodes/ClonableNode.h"

class StringNode : public ClonableNode<StringNode>
{
public:
    StringNode();
    void DrawImpl() final;
    void Update() final;
    void SpecialConstructFromJSON(const nlohmann::json& json) final;
    void SpecialSerialze(nlohmann::json& json) final;
    std::string GetNodeTypeName() final;
private:
    std::string string;
};