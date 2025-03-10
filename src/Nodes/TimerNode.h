#pragma once
#include "Nodes/Node.h"
#include "Nodes/Pin.h"
#include <chrono>

class TimerNode : public Node
{
public:
TimerNode();
    void Draw() final;
    void Update() final;
    std::vector<std::shared_ptr<Pin>> GetPins() final;
private:
    std::shared_ptr<Pin> outputPin;
    int repRate = 1000;
    std::chrono::time_point<std::chrono::high_resolution_clock> lastTriggerTime;
};