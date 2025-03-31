#pragma once
#include "Nodes/Node.h"
#include "Nodes/Pin.h"

class NumberNode : public Node
{
public:
    NumberNode(ax::NodeEditor::NodeId id);
    NumberNode(NumberNode& copy);
    std::shared_ptr<Node> Clone() final {return std::make_shared<NumberNode>(*this);};
    void Draw() final;
    void Update() final;
    std::vector<std::shared_ptr<Pin>> GetPins() final;
    void ConstructFromJSON(const nlohmann::json& json) final;
    void SpecialSerialze(nlohmann::json& json) final;
    std::string GetNodeTypeName() final;
private:
    std::shared_ptr<Pin> outputPin;
    std::shared_ptr<Pin> inputPin1;
    std::shared_ptr<Pin> inputPin2;
    int integer = 0;
    double floatingPoint = 0.0;
    bool isFloating;
};