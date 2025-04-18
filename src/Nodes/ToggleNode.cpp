#include "Nodes/ToggleNode.h"
#include "imgui_node_editor.h"
#include "misc/cpp/imgui_stdlib.h"


ToggleNode::ToggleNode(ax::NodeEditor::NodeId id) : Node(id)
{
    AddOutputPin("", Pin::PinType::Boolean);
}

std::string ToggleNode::GetNodeTypeName()
{
    return "ToggleNode";
}

void ToggleNode::DrawImpl()
{
    if(ImGui::RadioButton("Toggle", output))
    {
        output = !output;
    }
    
    ImGui::SameLine();
}

void ToggleNode::Update()
{
    outputPins[0]->value = output;
}

void ToggleNode::SpecialConstructFromJSON(const nlohmann::json& json)
{
    if(json.contains("active"))
    {
        output = json["active"];
    }
}

void ToggleNode::SpecialSerialze(nlohmann::json& json)
{
    json["active"] = output;
}
