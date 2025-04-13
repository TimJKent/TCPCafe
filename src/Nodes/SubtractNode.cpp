#include "Nodes/SubtractNode.h"
#include "imgui_node_editor.h"
#include "misc/cpp/imgui_stdlib.h"
#include <cmath>

SubtractNode::SubtractNode(ax::NodeEditor::NodeId id) : Node(id)
, outputPin(std::make_shared<Pin>("Diff", ax::NodeEditor::PinKind::Output, Pin::PinType::Number))
{
    AddInputPin();
    AddInputPin();
}

SubtractNode::SubtractNode(SubtractNode& copy) : Node(++NodeManager::globalId)
, outputPin(std::make_shared<Pin>(*copy.outputPin.get()))
{
    inputPins.clear();
    for(auto& pin : copy.inputPins)
    {
        inputPins.push_back(std::make_shared<Pin>(*pin.get()));
    }
}

std::string SubtractNode::GetNodeTypeName()
{
    return "SubtractNode";
}

void SubtractNode::Draw()
{
    ax::NodeEditor::BeginNode(id);
        ImGui::Text("Subtract");
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

void SubtractNode::Update()
{
    int iOutput = 0;
    double fOutput = 0.0f;
    int pinIndex = 0;
    
    bool outputAsFloat = false;

    for(std::shared_ptr<Pin> pin : GetPins())
    {
        if(pin->pinKind == ax::NodeEditor::PinKind::Input && pin->active && pin->any.has_value())
        {
            if(pin->any.type() == typeid(int))
            {
                if (pinIndex == 0)
                {
                    iOutput += std::any_cast<int>(pin->any);
                    fOutput += std::any_cast<int>(pin->any);
                }
                else
                {
                    iOutput -= std::any_cast<int>(pin->any);
                    fOutput -= std::any_cast<int>(pin->any);
                }
            }
            else if(pin->any.type() == typeid(double))
            {
                if (pinIndex == 0)
                {
                    iOutput += (int)std::round(std::any_cast<double>(pin->any));
                    fOutput += std::any_cast<double>(pin->any);
                }
                else
                {
                    iOutput -= (int)std::round(std::any_cast<double>(pin->any));
                    fOutput -= std::any_cast<double>(pin->any);
                }
                outputAsFloat = true;
            }
        }
        pinIndex++;
    }

    if(outputAsFloat)
    {
        outputPin->any = std::make_any<double>(fOutput);
    }
    else
    {
        outputPin->any = std::make_any<int>(iOutput);
    }

    if(inputPins[inputPins.size()-1]->active)
    {
        AddInputPin();
    }

    if(inputPins.size() > 2 && !inputPins[inputPins.size()-2]->active)
    {
        RemoveInputPin();
    }

    outputPin->active = true;
}

void SubtractNode::AddInputPin()
{
    inputPins.emplace_back(std::make_shared<Pin>("A", ax::NodeEditor::PinKind::Input, Pin::PinType::Number));
}

void SubtractNode::RemoveInputPin()
{
    inputPins.pop_back();
}

std::vector<std::shared_ptr<Pin>> SubtractNode::GetPins()
{
    std::vector<std::shared_ptr<Pin>> pins = inputPins;
    pins.emplace_back(outputPin);
    return pins;
}

void SubtractNode::ConstructFromJSON(const nlohmann::json& json)
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