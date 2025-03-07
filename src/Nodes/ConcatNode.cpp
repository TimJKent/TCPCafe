#include "Nodes/ConcatNode.h"
#include "imgui_node_editor.h"
#include "misc/cpp/imgui_stdlib.h"


ConcatNode::ConcatNode() : Node()
, stringAPin(std::make_shared<Pin>("String", ax::NodeEditor::PinKind::Input,Pin::PinType::String))
, stringBPin(std::make_shared<Pin>("String", ax::NodeEditor::PinKind::Input,Pin::PinType::String))
, outputPin(std::make_shared<Pin>("String",  ax::NodeEditor::PinKind::Output,Pin::PinType::String))
{

}

void ConcatNode::Draw()
{
    ax::NodeEditor::BeginNode(id);
        ImGui::Text("Concatenate");
        stringAPin->Draw();
        ImGui::SameLine();
        outputPin->Draw();
        stringBPin->Draw();
    ax::NodeEditor::EndNode();

    std::string output = "";

    if(stringAPin->active)
    {
        if(stringAPin->any.has_value() && stringAPin->any.type() == typeid(std::string))
        {
            output += std::any_cast<std::string>(stringAPin->any);
        }
    }

    if(stringBPin->active)
    {
        if(stringBPin->any.has_value() && stringBPin->any.type() == typeid(std::string))
        {
            output += std::any_cast<std::string>(stringBPin->any);
        }
    }

    outputPin->any = std::make_any<std::string>(output);
    outputPin->active = true;
}

std::vector<std::shared_ptr<Pin>> ConcatNode::GetPins()
{
    return {stringAPin, stringBPin, outputPin};
}