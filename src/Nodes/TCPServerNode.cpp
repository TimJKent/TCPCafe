#include "Nodes/TCPServerNode.h"
#include "imgui_node_editor.h"
#include "misc/cpp/imgui_stdlib.h"
#include <iostream>


TCPServerNode::TCPServerNode(ax::NodeEditor::NodeId id, std::shared_ptr<TCPServer> tcpServer) : Node(id)
, tcpServer(tcpServer)
{
    AddInputPin("Send", Pin::PinType::Boolean);
    AddInputPin("Message", Pin::PinType::Any);
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

void TCPServerNode::DrawImpl()
{
        ImGui::Text("TCP Server");

}

void TCPServerNode::Update()
{
    message = inputPins[1]->isConnected ? inputPins[1]->PinOutputToString() : "";

    if(inputPins[0]->isConnected && std::get<bool>(inputPins[0]->value)) 
    {
        Send();
    }
}