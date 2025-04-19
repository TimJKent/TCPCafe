#include "Nodes/SubtractNode.h"
#include "imgui_node_editor.h"
#include "misc/cpp/imgui_stdlib.h"
#include <cmath>

SubtractNode::SubtractNode(ax::NodeEditor::NodeId id) : ClonableNode<SubtractNode>(id)
{
    AddOutputPin("Diff", Pin::PinType::Number);
    AddInputPin("A", Pin::PinType::Number);
    AddInputPin("A", Pin::PinType::Number);
}

std::string SubtractNode::GetNodeTypeName()
{
    return "SubtractNode";
}

void SubtractNode::DrawImpl()
{
    ImGui::Text("Subtract");
}

void SubtractNode::Update()
{
    int iOutput = 0;
    double fOutput = 0.0f;
    int pinIndex = 0;
    
    bool outputAsFloat = false;

    for(std::shared_ptr<Pin> pin : inputPins)
    {
        if(pin->isConnected)
        {
            if(std::holds_alternative<int>(pin->value))
            {
                if (pinIndex == 0)
                {
                    iOutput += std::get<int>(pin->value);
                    fOutput += std::get<int>(pin->value);
                }
                else
                {
                    iOutput -= std::get<int>(pin->value);
                    fOutput -= std::get<int>(pin->value);
                }
            }
            else if(std::holds_alternative<double>(pin->value))
            {
                if (pinIndex == 0)
                {
                    iOutput += (int)std::round(std::get<double>(pin->value));
                    fOutput += std::get<double>(pin->value);
                }
                else
                {
                    iOutput -= (int)std::round(std::get<double>(pin->value));
                    fOutput -= std::get<double>(pin->value);
                }
                outputAsFloat = true;
            }
        }
        pinIndex++;
    }

    if(outputAsFloat)
    {
        outputPins[0]->value = fOutput;
    }
    else
    {
        outputPins[0]->value = iOutput;
    }

    if(inputPins[inputPins.size()-1]->isConnected)
    {
        AddInputPin("A", Pin::PinType::Number);
    }

    if(inputPins.size() > 2 && !inputPins[inputPins.size()-2]->isConnected)
    {
        RemoveInputPin();
    }
}