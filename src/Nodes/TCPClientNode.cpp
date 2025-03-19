#include "Nodes/TCPClientNode.h"
#include "imgui_node_editor.h"
#include "misc/cpp/imgui_stdlib.h"
#include <iostream>


TCPClientNode::TCPClientNode(std::shared_ptr<TCPClient> tcpClient) : Node()
, triggerSendPin(std::make_shared<Pin>("Send Trigger", ax::NodeEditor::PinKind::Input, Pin::PinType::Trigger))
, stringPin(std::make_shared<Pin>("Send String", ax::NodeEditor::PinKind::Input, Pin::PinType::Any))
, tcpClient(tcpClient)
{

}

void TCPClientNode::Send()
{
    if(tcpClient && tcpClient->IsConnected())
    {
        tcpClient->SendMessageA(message);
    }
}

void TCPClientNode::Draw()
{
    ax::NodeEditor::BeginNode(id);
        ImGui::Text("TCP Client");
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

std::vector<std::shared_ptr<Pin>> TCPClientNode::GetPins()
{
    return {triggerSendPin, stringPin};
}