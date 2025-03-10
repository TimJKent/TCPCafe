#include "Nodes/ToStringNode.h"
#include "imgui_node_editor.h"
#include "misc/cpp/imgui_stdlib.h"


ToStringNode::ToStringNode() : Node()
, inputPin(std::make_shared<Pin>("Any", ax::NodeEditor::PinKind::Input,Pin::PinType::Any))
, outputPin(std::make_shared<Pin>("String",  ax::NodeEditor::PinKind::Output,Pin::PinType::String))
{

}

void ToStringNode::Draw()
{
    ax::NodeEditor::BeginNode(id);
        ImGui::Text("ToString");
        inputPin->Draw();
        ImGui::SameLine();
        outputPin->Draw();
    ax::NodeEditor::EndNode();
}

void ToStringNode::Update()
{
    std::string output = "";
    if(inputPin->active)
    {
        if(inputPin->any.has_value())
        {
            if(inputPin->any.type() == typeid(std::string))
            {
                output = std::any_cast<std::string>(inputPin->any);
            }
            else if(inputPin->any.type() == typeid(int))
            {
                output = std::to_string(std::any_cast<int>(inputPin->any));
            }
        }
    }

    outputPin->any = std::make_any<std::string>(output);
    outputPin->active = true;
}

std::vector<std::shared_ptr<Pin>> ToStringNode::GetPins()
{
    return {inputPin, outputPin};
}