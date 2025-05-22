#include "Nodes/BooleanDisplayNode.h"
#include "imgui_node_editor.h"
#include "misc/cpp/imgui_stdlib.h"
#include <iostream>


BooleanDisplayNode::BooleanDisplayNode() : ClonableNode<BooleanDisplayNode>()
{
    AddInputPin("in", Pin::PinType::Boolean);
}

std::string BooleanDisplayNode::GetNodeTypeName()
{
    return "BooleanDisplayNode";
}

void BooleanDisplayNode::DrawImpl()
{
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 p1 = ImGui::GetCursorScreenPos();
    ImVec2 p2 = ImVec2(p1.x + 100, p1.y + 100);
    ImGui::Dummy({100,100});

    bool displayWhite = inputPins[0]->isConnected && std::get<bool>(inputPins[0]->value);
    ImColor color = displayWhite ? IM_COL32(255, 255, 255, 255) : IM_COL32(0, 0, 0, 255);
    draw_list->AddRectFilled(p1, p2, color, 0.0f, 0);
}