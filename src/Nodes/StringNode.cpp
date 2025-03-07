#include "Nodes/StringNode.h"
#include "imgui_node_editor.h"
#include "misc/cpp/imgui_stdlib.h"


StringNode::StringNode() : Node()
, outputPin(std::make_shared<Pin>("String", ax::NodeEditor::PinKind::Output,Pin::PinType::String))
{

}

void StringNode::Draw()
{
    ax::NodeEditor::BeginNode(id);
        ImGui::Text("String");
        ImGui::SetNextItemWidth(200);
        ImGui::InputText("##String", &string);
        ImGui::SameLine();
        outputPin->Draw();
    ax::NodeEditor::EndNode();

    outputPin->any = std::make_any<std::string>(string);
    outputPin->active = true;
}

std::vector<std::shared_ptr<Pin>> StringNode::GetPins()
{
    return {outputPin};
}