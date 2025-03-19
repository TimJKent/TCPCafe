#include "Nodes/TCPServerNode.h"
#include "imgui_node_editor.h"
#include "misc/cpp/imgui_stdlib.h"
#include <iostream>


TCPServerNode::TCPServerNode(std::shared_ptr<TCPServer> tcpServer) : Node()
, triggerSendPin(std::make_shared<Pin>("Send Trigger", ax::NodeEditor::PinKind::Input, Pin::PinType::Trigger))
, stringPin(std::make_shared<Pin>("Send String", ax::NodeEditor::PinKind::Input, Pin::PinType::Any))
, tcpServer(tcpServer)
{

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