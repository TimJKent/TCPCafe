#pragma once
#include "Nodes/ClonableNode.h"

class NumberNode : public ClonableNode<NumberNode>
{
public:
    NumberNode();
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