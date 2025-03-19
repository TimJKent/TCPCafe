#include "Nodes/PrintNode.h"
#include "imgui_node_editor.h"
#include "misc/cpp/imgui_stdlib.h"
#include <iostream>


PrintNode::PrintNode() : Node()
, triggerPin(std::make_shared<Pin>("Trigger", ax::NodeEditor::PinKind::Input, Pin::PinType::Trigger))
, stringPin(std::make_shared<Pin>("String", ax::NodeEditor::PinKind::Input, Pin::PinType::Any))
{

}

void PrintNode::Print()
{
    std::cout << message << std::endl;
}

void PrintNode::Draw()
{
    ax::NodeEditor::BeginNode(id);
        ImGui::Text("Print Message");
        stringPin->Draw();
        triggerPin->Draw();
    ax::NodeEditor::EndNode();

    if(stringPin->active)
    {
            message = stringPin->PinOutputToString();
    }else
    {
        message = "";
    }

    if(triggerPin->active)
    {
        Print();
    }
}

std::vector<std::shared_ptr<Pin>> PrintNode::GetPins()
{
    return {triggerPin, stringPin};
}