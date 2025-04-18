#include "Nodes/PrintNode.h"
#include "imgui_node_editor.h"
#include "misc/cpp/imgui_stdlib.h"
#include <iostream>


PrintNode::PrintNode(ax::NodeEditor::NodeId id) : Node(id)
{
    AddInputPin("Trigger", Pin::PinType::Boolean);
    AddInputPin("String", Pin::PinType::Any);
}

void PrintNode::Print()
{
    std::cout << message << std::endl;
}

std::string PrintNode::GetNodeTypeName()
{
    return "PrintNode";
}

void PrintNode::DrawImpl()
{
    ImGui::Text("Print Message");
}

void PrintNode::Update()
{
    if(inputPins[1]->isConnected)
    {
        message = inputPins[1]->PinOutputToString();
    }
    else
    {
        message = "";
    }

    if(inputPins[0]->isConnected && std::get<bool>(inputPins[0]->value))
    {
        if(risingEdge)
        {
            risingEdge = false;
            Print();
        }
    }else
    {
        risingEdge = true;
    }
}