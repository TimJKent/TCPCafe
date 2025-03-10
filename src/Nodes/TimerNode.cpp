#include "Nodes/TimerNode.h"
#include "imgui_node_editor.h"
#include "misc/cpp/imgui_stdlib.h"


TimerNode::TimerNode() : Node()
, outputPin(std::make_shared<Pin>("Trigger", ax::NodeEditor::PinKind::Output, Pin::PinType::Trigger))
{
    lastTriggerTime = std::chrono::high_resolution_clock::now();
}

void TimerNode::Draw()
{
    ax::NodeEditor::BeginNode(id);
        ImGui::Text("Timer");
        ImGui::SetNextItemWidth(150);
        ImGui::InputInt("MS", &repRate);
        outputPin->active = false;
        outputPin->Draw();
    ax::NodeEditor::EndNode();
}

void TimerNode::Update()
{
    std::chrono::time_point<std::chrono::high_resolution_clock> now = std::chrono::high_resolution_clock::now();
    std::chrono::milliseconds msEllapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now- lastTriggerTime);
    
    if(msEllapsed.count() >= repRate)
    {
        outputPin->active = true;
        lastTriggerTime = now;
    }
    else
    {
        outputPin->active = false;
    }
}


std::vector<std::shared_ptr<Pin>> TimerNode::GetPins()
{
    return {outputPin};
}