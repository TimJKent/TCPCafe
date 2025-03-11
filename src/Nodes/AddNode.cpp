#include "Nodes/AddNode.h"
#include "imgui_node_editor.h"
#include "misc/cpp/imgui_stdlib.h"


AddNode::AddNode() : Node()
, inputPin1(std::make_shared<Pin>("Abc", ax::NodeEditor::PinKind::Input, Pin::PinType::Int))
, inputPin2(std::make_shared<Pin>("Bbc", ax::NodeEditor::PinKind::Input, Pin::PinType::Int))
, outputPin(std::make_shared<Pin>("Sum", ax::NodeEditor::PinKind::Output, Pin::PinType::Int))
{

}

void AddNode::Draw()
{
    ax::NodeEditor::BeginNode(id);
        ImGui::Text("Add");
        inputPin1->Draw();
        ImGui::SameLine();
        outputPin->Draw();
        inputPin2->Draw();
    ax::NodeEditor::EndNode();
}

void AddNode::Update()
{
    int output = 0;
    
    if(inputPin1->active && inputPin1->any.has_value() && inputPin1->any.type() == typeid(int))
    {
        output += std::any_cast<int>(inputPin1->any);
    }
    if(inputPin2->active && inputPin2->any.has_value() && inputPin2->any.type() == typeid(int))
    {
        output += std::any_cast<int>(inputPin2->any);
    }
    
    outputPin->any = std::make_any<int>(output);
    outputPin->active = true;
}

std::vector<std::shared_ptr<Pin>> AddNode::GetPins()
{
    return {inputPin1, inputPin2, outputPin};
}