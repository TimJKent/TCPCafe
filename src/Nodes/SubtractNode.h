#pragma once
#include "Nodes/Node.h"
#include "Nodes/Pin.h"

class SubtractNode : public Node
{
public:
    SubtractNode(ax::NodeEditor::NodeId id);
    SubtractNode(SubtractNode& copy);
    std::shared_ptr<Node> Clone() final {return std::make_shared<SubtractNode>(*this);};
    void Draw() final;
    void Update() final;
    std::vector<std::shared_ptr<Pin>> GetPins() final;
    void ConstructFromJSON(const nlohmann::json& json) final;
    void AddInputPin();
    void RemoveInputPin();
    std::string GetNodeTypeName() final;
private:
    std::vector<std::shared_ptr<Pin>> inputPins;
    std::shared_ptr<Pin> outputPin;
};