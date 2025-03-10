#include "Nodes/ClientSendNode.h"
#include "imgui_node_editor.h"
#include "misc/cpp/imgui_stdlib.h"
#include <iostream>


ClientSendNode::ClientSendNode(std::shared_ptr<TCPClient> tcpClient) : Node()
, triggerSendPin(std::make_shared<Pin>("Send", ax::NodeEditor::PinKind::Input, Pin::PinType::Trigger))
, stringPin(std::make_shared<Pin>("String", ax::NodeEditor::PinKind::Input, Pin::PinType::String))
, tcpClient(tcpClient)
{

}

void ClientSendNode::Send()
{
    if(tcpClient && tcpClient->IsConnected())
    {
        tcpClient->SendMessageA(message);
    }
}

void ClientSendNode::Draw()
{
    ax::NodeEditor::BeginNode(id);
        ImGui::Text("Print Message");
        stringPin->Draw();
        triggerSendPin->Draw();
    ax::NodeEditor::EndNode();

    if(stringPin->active)
    {
        if(stringPin->any.has_value() && stringPin->any.type() == typeid(std::string))
        {
            message = std::any_cast<std::string>(stringPin->any);
        }
    }else
    {
        message = "";
    }

    if(triggerSendPin->active)
    {
        Send();
    }
}

std::vector<std::shared_ptr<Pin>> ClientSendNode::GetPins()
{
    return {triggerSendPin, stringPin};
}