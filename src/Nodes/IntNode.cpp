#include "Nodes/IntNode.h"
#include "imgui_node_editor.h"
#include "misc/cpp/imgui_stdlib.h"


IntNode::IntNode() : Node()
, outputPin(std::make_shared<Pin>("Int", ax::NodeEditor::PinKind::Output,Pin::PinType::Int))
, inputPin1(std::make_shared<Pin>("Set Int", ax::NodeEditor::PinKind::Input,Pin::PinType::Int))
, inputPin2(std::make_shared<Pin>("Set", ax::NodeEditor::PinKind::Input,Pin::PinType::Trigger))
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
        inputPin1->Draw();
        inputPin2->Draw();
    ax::NodeEditor::EndNode();
}

void IntNode::Update()
{
    if(inputPin1->active && inputPin2->active && inputPin1->any.has_value() && inputPin1->any.type() == typeid(int))
    {
        integer = std::any_cast<int>(inputPin1->any);
    }

    outputPin->any = std::make_any<int>(integer);
    outputPin->active = true;
}

std::vector<std::shared_ptr<Pin>> IntNode::GetPins()
{
    return {inputPin1, inputPin2, outputPin};
}