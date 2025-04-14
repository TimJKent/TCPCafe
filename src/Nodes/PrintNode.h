#pragma once
#include "Nodes/Node.h"
#include "Nodes/Pin.h"

class PrintNode : public Node
{
public:
    PrintNode(ax::NodeEditor::NodeId id);
    PrintNode(PrintNode& copy);
    std::shared_ptr<Node> Clone() final {return std::make_shared<PrintNode>(*this);};
    void Draw() final;
    void Update() final;
    void Print();
    std::vector<std::shared_ptr<Pin>> GetPins() final;
    void ConstructFromJSON(const nlohmann::json& json) final;
    std::string GetNodeTypeName() final;
private:
    std::shared_ptr<Pin> triggerPin;
    std::shared_ptr<Pin> stringPin;
    std::string message;
};