#include "Nodes/AddNode.h"
#include "imgui_node_editor.h"
#include "misc/cpp/imgui_stdlib.h"
#include <cmath>

AddNode::AddNode(ax::NodeEditor::NodeId id) : Node(id)
, outputPin(std::make_shared<Pin>("Sum", ax::NodeEditor::PinKind::Output, Pin::PinType::Number))
{
    AddInputPin();
    AddInputPin();
}

AddNode::AddNode(AddNode& copy) : Node(++NodeManager::globalId)
, outputPin(std::make_shared<Pin>(*copy.outputPin.get()))
{
    inputPins.clear();
    for(auto& pin : copy.inputPins)
    {
        inputPins.push_back(std::make_shared<Pin>(*pin.get()));
    }
}


std::string AddNode::GetNodeTypeName()
{
    return "AddNode";
}

void AddNode::Draw()
{
    ax::NodeEditor::BeginNode(id);
        ImGui::Text("Add");
        auto drawList  = ImGui::GetWindowDrawList();
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

void AddNode::Update()
{
    int iOutput = 0;
    double fOutput = 0.0f;
    
    bool outputAsFloat = false;

    for(std::shared_ptr<Pin> pin : GetPins())
    {
        if(pin->pinKind == ax::NodeEditor::PinKind::Input)
        {
            if(std::holds_alternative<int>(pin->value))
            {
                iOutput += std::get<int>(pin->value);
                fOutput += std::get<int>(pin->value);
            }
            if(std::holds_alternative<double>(pin->value))
            {
                iOutput += (int)std::round(std::get<double>(pin->value));
                fOutput += std::get<double>(pin->value);
                outputAsFloat = true;
            }
        }
    }

    outputPin->value =  outputAsFloat ? fOutput : iOutput;

    if(inputPins[inputPins.size()-1]->isConnected)
    {
        AddInputPin();
    }

    if(inputPins.size() > 2 && !inputPins[inputPins.size()-2]->isConnected)
    {
        RemoveInputPin();
    }
}

void AddNode::AddInputPin()
{
    inputPins.emplace_back(std::make_shared<Pin>("A", ax::NodeEditor::PinKind::Input, Pin::PinType::Number));
}

void AddNode::RemoveInputPin()
{
    inputPins.pop_back();
}

std::vector<std::shared_ptr<Pin>> AddNode::GetPins()
{
    std::vector<std::shared_ptr<Pin>> pins = inputPins;
    pins.emplace_back(outputPin);
    return pins;
}

void AddNode::ConstructFromJSON(const nlohmann::json& json)
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