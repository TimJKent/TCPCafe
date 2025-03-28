#include "Nodes/ButtonNode.h"
#include "imgui_node_editor.h"
#include "misc/cpp/imgui_stdlib.h"


ButtonNode::ButtonNode(ax::NodeEditor::NodeId id) : Node(id)
, outputPin(std::make_shared<Pin>("", ax::NodeEditor::PinKind::Output, Pin::PinType::Trigger))
{

}

std::string ButtonNode::GetNodeTypeName()
{
    return "ButtonNode";
}

void ButtonNode::Draw()
{
    ax::NodeEditor::BeginNode(id);
        ImGui::Text("Button");
        if(ImGui::Button("Trigger"))
        {
            outputPin->active = true;
        }
        else
        {
            outputPin->active = false;
        }
        ImGui::SameLine();
        outputPin->Draw();
    ax::NodeEditor::EndNode();
    
}

std::vector<std::shared_ptr<Pin>> ButtonNode::GetPins()
{
    return {outputPin};
}

void ButtonNode::ConstructFromJSON(const nlohmann::json& json)
{
    for (auto& [key, val] : json["pins"].items())
    {
        std::shared_ptr<Pin> pin = std::make_shared<Pin>(val);
        if(pin->pinKind == ax::NodeEditor::PinKind::Output)
        {
            outputPin = pin;
        }
    }
}