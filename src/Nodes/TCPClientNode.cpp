#include "Nodes/TCPClientNode.h"
#include "imgui_node_editor.h"
#include "misc/cpp/imgui_stdlib.h"
#include <iostream>


TCPClientNode::TCPClientNode(ax::NodeEditor::NodeId id, std::shared_ptr<TCPClient> tcpClient) : ClonableNode<TCPClientNode>(id)
, tcpClient(tcpClient)
{
    AddInputPin("Send", Pin::PinType::Boolean);
    AddInputPin("Message", Pin::PinType::Any);
}

std::string TCPClientNode::GetNodeTypeName()
{
    return "TCPClientNode";
}

void TCPClientNode::Send()
{
    if(tcpClient && tcpClient->IsConnected())
    {
        tcpClient->SendMessageA(message);
    }
}

void TCPClientNode::DrawImpl()
{
    ImGui::Text("TCP Client");
}

void TCPClientNode::Update()
{
    message = inputPins[1]->isConnected ? inputPins[1]->PinOutputToString() : "";

    
    if(inputPins[0]->isConnected && std::get<bool>(inputPins[0]->value))
    {
        if(risingEdge)
        {
            risingEdge = false;
            Send();
        }
    }else
    {
        risingEdge = true;
    }
}