#include "Nodes/ButtonNode.h"
#include "imgui_node_editor.h"
#include "misc/cpp/imgui_stdlib.h"

ButtonNode::ButtonNode() : ClonableNode<ButtonNode>()
{
    AddOutputPin("", Pin::PinType::Boolean);
}

std::string ButtonNode::GetNodeTypeName()
{
    return "ButtonNode";
}

void ButtonNode::DrawImpl()
{
    ImGui::Text("Button");
    outputPins[0]->value = ImGui::Button("Trigger");
    ImGui::SameLine();
}