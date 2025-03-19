#include "Nodes/ConcatNode.h"
#include "imgui_node_editor.h"
#include "misc/cpp/imgui_stdlib.h"


ConcatNode::ConcatNode() : Node()
, stringAPin(std::make_shared<Pin>("String", ax::NodeEditor::PinKind::Input,Pin::PinType::Any))
, stringBPin(std::make_shared<Pin>("String", ax::NodeEditor::PinKind::Input,Pin::PinType::Any))
, outputPin(std::make_shared<Pin>("String",  ax::NodeEditor::PinKind::Output,Pin::PinType::Any))
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
        output +=stringAPin->PinOutputToString();
    }

    if(stringBPin->active)
    {
        output +=stringBPin->PinOutputToString();
    }

    outputPin->any = std::make_any<std::string>(output);
    outputPin->active = true;
}

std::vector<std::shared_ptr<Pin>> ConcatNode::GetPins()
{
    return {stringAPin, stringBPin, outputPin};
}