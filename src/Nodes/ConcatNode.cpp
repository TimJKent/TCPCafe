#include "Nodes/ConcatNode.h"
#include "imgui_node_editor.h"
#include "misc/cpp/imgui_stdlib.h"
#include <cmath>

ConcatNode::ConcatNode() : Node()
, outputPin(std::make_shared<Pin>("Out", ax::NodeEditor::PinKind::Output, Pin::PinType::Any))
{
    AddInputPin();
    AddInputPin();
}

void ConcatNode::Draw()
{
    ax::NodeEditor::BeginNode(id);
        ImGui::Text("Concatenate");

        inputPins[0]->Draw();
        ImGui::SameLine();
        outputPin->Draw();
        inputPins[1]->Draw();
        for(int i = 2; i < inputPins.size(); i++)
        {
            inputPins[i]->Draw();
        }
    ax::NodeEditor::EndNode();
}

void ConcatNode::Update()
{
    std::string output = "";

    for(std::shared_ptr<Pin> pin : GetPins())
    {
        if(pin->pinKind == ax::NodeEditor::PinKind::Input && pin->active && pin->any.has_value())
        {
            output += pin->PinOutputToString();
        }
    }

    outputPin->any = std::make_any<std::string>(output);

    if(inputPins[inputPins.size()-1]->active)
    {
        AddInputPin();
    }

    if(inputPins.size() > 2 &&!inputPins[inputPins.size()-2]->active)
    {
        RemoveInputPin();
    }

    outputPin->active = true;
}

void ConcatNode::AddInputPin()
{
    inputPins.emplace_back(std::make_shared<Pin>("A", ax::NodeEditor::PinKind::Input, Pin::PinType::Any));
}

void ConcatNode::RemoveInputPin()
{
    inputPins.pop_back();
}

std::vector<std::shared_ptr<Pin>> ConcatNode::GetPins()
{
    std::vector<std::shared_ptr<Pin>> pins = inputPins;
    pins.emplace_back(outputPin);
    return pins;
}