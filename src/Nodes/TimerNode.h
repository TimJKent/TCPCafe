#pragma once
#include "Nodes/Node.h"
#include <chrono>

class TimerNode : public Node
{
public:
    TimerNode(ax::NodeEditor::NodeId id);
    std::shared_ptr<Node> Clone() final {return std::make_shared<TimerNode>(*this);};
    void DrawImpl() final;
    void Update() final;
    void SpecialConstructFromJSON(const nlohmann::json& json) final;
    void SpecialSerialze(nlohmann::json& json) final;
    std::string GetNodeTypeName() final;
private:
    int repRate = 1000;
    std::chrono::time_point<std::chrono::high_resolution_clock> lastTriggerTime;
};