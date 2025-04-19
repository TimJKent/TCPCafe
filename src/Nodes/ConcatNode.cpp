#include "Nodes/ConcatNode.h"
#include "imgui_node_editor.h"
#include "misc/cpp/imgui_stdlib.h"
#include <cmath>

ConcatNode::ConcatNode(ax::NodeEditor::NodeId id) : ClonableNode<ConcatNode>(id)
{
    AddOutputPin("Out", Pin::PinType::Any);
    AddInputPin("A", Pin::PinType::Any);
    AddInputPin("A", Pin::PinType::Any);
}

std::string ConcatNode::GetNodeTypeName()
{
    return "ConcatNode";
}

void ConcatNode::DrawImpl()
{
        ImGui::Text("Concatenate");
}

void ConcatNode::Update()
{
    std::string output = "";

    for(std::shared_ptr<Pin> pin : inputPins)
    {
        if(pin->isConnected)
        {
            output += pin->PinOutputToString();
        }
    }

    outputPins[0]->value = output;

    if(inputPins[inputPins.size()-1]->isConnected)
    {
        AddInputPin("A", Pin::PinType::Any);
    }

    if(inputPins.size() > 2 &&!inputPins[inputPins.size()-2]->isConnected)
    {
        RemoveInputPin();
    }
}