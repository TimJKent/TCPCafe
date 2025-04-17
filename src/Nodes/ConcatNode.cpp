#include "Nodes/ConcatNode.h"
#include "imgui_node_editor.h"
#include "misc/cpp/imgui_stdlib.h"
#include <cmath>

ConcatNode::ConcatNode(ax::NodeEditor::NodeId id) : Node(id)
, outputPin(std::make_shared<Pin>("Out", ax::NodeEditor::PinKind::Output, Pin::PinType::Any))
{
    AddInputPin();
    AddInputPin();
}

ConcatNode::ConcatNode(ConcatNode& copy) : Node(++NodeManager::globalId)
, outputPin(std::make_shared<Pin>(*copy.outputPin.get()))
{
    inputPins.clear();
    for(auto& pin : copy.inputPins)
    {
        inputPins.push_back(std::make_shared<Pin>(*pin.get()));
    }
}


std::string ConcatNode::GetNodeTypeName()
{
    return "ConcatNode";
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
        if(pin->pinKind == ax::NodeEditor::PinKind::Input && pin->isConnected)
        {
            output += pin->PinOutputToString();
        }
    }

    outputPin->value = output;

    if(inputPins[inputPins.size()-1]->isConnected)
    {
        AddInputPin();
    }

    if(inputPins.size() > 2 &&!inputPins[inputPins.size()-2]->isConnected)
    {
        RemoveInputPin();
    }
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

void ConcatNode::ConstructFromJSON(const nlohmann::json& json)
{
    inputPins.clear();
    for (auto& [key, val] : json["pins"].items())
    {
        std::shared_ptr<Pin> pin = std::make_shared<Pin>(val);
        if(pin->pinKind == ax::NodeEditor::PinKind::Input)
        {
            inputPins.emplace_back(pin);
        }else
        {
            outputPin = pin;
        }
    }
}