#include "Nodes/AddNode.h"
#include "imgui_node_editor.h"
#include "misc/cpp/imgui_stdlib.h"
#include <cmath>

AddNode::AddNode() : Node()
, inputPin1(std::make_shared<Pin>("A", ax::NodeEditor::PinKind::Input, Pin::PinType::Number))
, inputPin2(std::make_shared<Pin>("B", ax::NodeEditor::PinKind::Input, Pin::PinType::Number))
, outputPin(std::make_shared<Pin>("Sum(1)", ax::NodeEditor::PinKind::Output, Pin::PinType::Number))
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
    int iOutput = 0;
    float fOutput = 0.0f;
    
    bool outputAsFloat = false;

    for(std::shared_ptr<Pin> pin : GetPins())
    {
        if(pin->pinKind == ax::NodeEditor::PinKind::Input && pin->active && pin->any.has_value())
        {
            if(pin->any.type() == typeid(int))
            {
                iOutput += std::any_cast<int>(pin->any);
                fOutput += std::any_cast<int>(pin->any);
            }
            else if(pin->any.type() == typeid(float))
            {
                iOutput += (int)std::round(std::any_cast<float>(pin->any));
                fOutput += std::any_cast<float>(pin->any);
                outputAsFloat = true;
            }
        }
    }

    if(outputAsFloat)
    {
        outputPin->any = std::make_any<float>(fOutput);
    }
    else
    {
        outputPin->any = std::make_any<int>(iOutput);
    }

    outputPin->active = true;
}

std::vector<std::shared_ptr<Pin>> AddNode::GetPins()
{
    return {inputPin1, inputPin2, outputPin};
}