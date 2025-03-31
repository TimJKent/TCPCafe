#pragma once
#include "Nodes/Node.h"
#include "Nodes/Pin.h"
#include <chrono>

class TimerNode : public Node
{
public:
    TimerNode(ax::NodeEditor::NodeId id);
    TimerNode(TimerNode& copy);
    std::shared_ptr<Node> Clone() final {return std::make_shared<TimerNode>(*this);};
    void Draw() final;
    void Update() final;
    std::vector<std::shared_ptr<Pin>> GetPins() final;
    void ConstructFromJSON(const nlohmann::json& json) final;
    void SpecialSerialze(nlohmann::json& json) final;
    std::string GetNodeTypeName() final;
private:
    std::shared_ptr<Pin> outputPin;
    int repRate = 1000;
    std::chrono::time_point<std::chrono::high_resolution_clock> lastTriggerTime;
};