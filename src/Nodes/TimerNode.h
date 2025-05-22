#pragma once
#include "Nodes/ClonableNode.h"
#include <chrono>

class TimerNode : public ClonableNode<TimerNode>
{
public:
    TimerNode();
    void DrawImpl() final;
    void Update() final;
    void SpecialConstructFromJSON(const nlohmann::json& json) final;
    void SpecialSerialze(nlohmann::json& json) final;
    std::string GetNodeTypeName() final;
private:
    int repRate = 1000;
    std::chrono::time_point<std::chrono::high_resolution_clock> lastTriggerTime;
};