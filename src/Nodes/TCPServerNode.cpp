#include "Nodes/TCPServerNode.h"
#include "imgui_node_editor.h"
#include "misc/cpp/imgui_stdlib.h"
#include <iostream>


TCPServerNode::TCPServerNode(ax::NodeEditor::NodeId id, std::shared_ptr<TCPServer> tcpServer) : Node(id)
, triggerSendPin(std::make_shared<Pin>("Send Trigger", ax::NodeEditor::PinKind::Input, Pin::PinType::Trigger))
, stringPin(std::make_shared<Pin>("Send String", ax::NodeEditor::PinKind::Input, Pin::PinType::Any))
, tcpServer(tcpServer)
{

}

std::string TCPServerNode::GetNodeTypeName()
{
    return "TCPServerNode";
}


void TCPServerNode::Send()
{
    if(tcpServer && tcpServer->IsListening())
    {
        tcpServer->SendMessageA(message);
    }
}

void TCPServerNode::Draw()
{
    ax::NodeEditor::BeginNode(id);
        ImGui::Text("TCP Server");
        stringPin->Draw();
        triggerSendPin->Draw();
    ax::NodeEditor::EndNode();

    if(stringPin->active)
    {
        message = stringPin->PinOutputToString();
    }else
    {
        message = "";
    }

    if(triggerSendPin->active)
    {
        Send();
    }
}

std::vector<std::shared_ptr<Pin>> TCPServerNode::GetPins()
{
    return {triggerSendPin, stringPin};
}

void TCPServerNode::ConstructFromJSON(const nlohmann::json& json)
{
    for (auto& [key, val] : json["pins"].items())
    {
        std::shared_ptr<Pin> pin = std::make_shared<Pin>(val);
        if(pin->GetName() == "Send Trigger")
        {
            triggerSendPin = pin;
        }else if(pin->GetName() == "Send String")
        {
            stringPin = pin;
        }
    }
}