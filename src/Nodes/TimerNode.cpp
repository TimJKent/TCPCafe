#include "Nodes/TimerNode.h"
#include "imgui_node_editor.h"
#include "misc/cpp/imgui_stdlib.h"


TimerNode::TimerNode(ax::NodeEditor::NodeId id) : ClonableNode<TimerNode>(id)
{
    AddOutputPin("Trigger", Pin::PinType::Boolean);
    lastTriggerTime = std::chrono::high_resolution_clock::now();
}

std::string TimerNode::GetNodeTypeName()
{
    return "TimerNode";
}

void TimerNode::DrawImpl()
{
        ImGui::Text("Timer");
        ImGui::SetNextItemWidth(150);
        ImGui::InputInt("MS", &repRate);
        ImGui::NewLine();
        ImGui::SameLine(70);
}

void TimerNode::Update()
{
    std::chrono::time_point<std::chrono::high_resolution_clock> now = std::chrono::high_resolution_clock::now();
    std::chrono::milliseconds msEllapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now- lastTriggerTime);
    
    if(msEllapsed.count() >= repRate)
    {
        lastTriggerTime = now;
        outputPins[0]->value = true;
    }
    else
    {
        outputPins[0]->value = false;
    }
}


void TimerNode::SpecialConstructFromJSON(const nlohmann::json& json)
{
    if(json.contains("repRate"))
    {
        repRate = json["repRate"];
    }
}

void TimerNode::SpecialSerialze(nlohmann::json& json)
{
    json["repRate"] = repRate;
}