#pragma once
#include "Nodes/Node.h"
#include "Nodes/Pin.h"

class ButtonNode : public Node
{
public:
    ButtonNode(ax::NodeEditor::NodeId id);
    ButtonNode(ButtonNode& copy);
    std::shared_ptr<Node> Clone() final {return std::make_shared<ButtonNode>(*this);};
    void Draw() final;
    std::vector<std::shared_ptr<Pin>> GetPins() final;
    void ConstructFromJSON(const nlohmann::json& json) final;
    std::string GetNodeTypeName() final;
private:
    std::shared_ptr<Pin> outputPin;
};