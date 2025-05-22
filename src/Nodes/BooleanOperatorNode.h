#pragma once
#include "Nodes/ClonableNode.h"
#include <Nodes/ClonableNode.h>

class BooleanOperatorNode : public ClonableNode<BooleanOperatorNode>
{
enum OPERATOR
{
    AND, OR, NOT, XOR
};
public:
    BooleanOperatorNode();
    void DrawImpl() final;
    void Update() final;
    void SpecialConstructFromJSON(const nlohmann::json& json) final;
    void SpecialSerialze(nlohmann::json& json) final;
    std::string GetNodeTypeName() final;
    bool GetOutputForAND();
    bool GetOutputForOR();
    bool GetOutputForNOT();
    bool GetOutputForXOR();
private:
    int selected = 0;
};