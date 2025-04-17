#include "Nodes/PrintNode.h"
#include "imgui_node_editor.h"
#include "misc/cpp/imgui_stdlib.h"
#include <iostream>


PrintNode::PrintNode(ax::NodeEditor::NodeId id) : Node(id)
, triggerPin(std::make_shared<Pin>("Trigger", ax::NodeEditor::PinKind::Input, Pin::PinType::Boolean))
, stringPin(std::make_shared<Pin>("String", ax::NodeEditor::PinKind::Input, Pin::PinType::Any))
{

}

PrintNode::PrintNode(PrintNode& copy) : Node(++NodeManager::globalId)
, triggerPin(std::make_shared<Pin>(*copy.triggerPin.get()))
, stringPin(std::make_shared<Pin>(*copy.stringPin.get()))
{
}

void PrintNode::Print()
{
    std::cout << message << std::endl;
}

std::string PrintNode::GetNodeTypeName()
{
    return "PrintNode";
}

void PrintNode::Draw()
{
    ax::NodeEditor::BeginNode(id);
        ImGui::Text("Print Message");
        stringPin->Draw();
        triggerPin->Draw();
    ax::NodeEditor::EndNode();
}

void PrintNode::Update()
{
    static bool risingEdge = true;
    if(stringPin->isConnected)
    {
        message = stringPin->PinOutputToString();
    }
    else
    {
        message = "";
    }

    if(triggerPin->isConnected && std::get<bool>(triggerPin->value))
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

std::vector<std::shared_ptr<Pin>> PrintNode::GetPins()
{
    return {triggerPin, stringPin};
}

void PrintNode::ConstructFromJSON(const nlohmann::json& json)
{
    for (auto& [key, val] : json["pins"].items())
    {
        std::shared_ptr<Pin> pin = std::make_shared<Pin>(val);
        if(pin->GetName() == "Trigger")
        {
            triggerPin = pin;
        }else if(pin->GetName() == "String")
        {
            stringPin = pin;
        }
    }
}