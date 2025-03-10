#include "Nodes/IntNode.h"
#include "imgui_node_editor.h"
#include "misc/cpp/imgui_stdlib.h"


IntNode::IntNode() : Node()
, outputPin(std::make_shared<Pin>("Int", ax::NodeEditor::PinKind::Output,Pin::PinType::Int))
{

}

void IntNode::Draw()
{
    ax::NodeEditor::BeginNode(id);
        ImGui::Text("Int");
        ImGui::SetNextItemWidth(200);
        ImGui::InputInt("##Int", &integer);
        ImGui::SameLine();
        outputPin->Draw();
    ax::NodeEditor::EndNode();

    outputPin->any = std::make_any<int>(integer);
    outputPin->active = true;
}

std::vector<std::shared_ptr<Pin>> IntNode::GetPins()
{
    return {outputPin};
}