#pragma once
#include "Nodes/Node.h"

class BooleanOperatorNode : public Node
{
enum OPERATOR
{
    AND, OR, NOT, XOR
};
public:
    BooleanOperatorNode(ax::NodeEditor::NodeId id);
    std::shared_ptr<Node> Clone() final {return std::make_shared<BooleanOperatorNode>(*this);};
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