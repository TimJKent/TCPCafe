#include "Nodes/NodeManager.h"
#include "Nodes/Node.h"
#include "Nodes/Pin.h"

#include <iostream>
#include <algorithm>
#include <fstream>

#include "Nodes/PrintNode.h"
#include "Nodes/PrintNode.h"
#include "Nodes/ButtonNode.h"
#include "Nodes/StringNode.h"
#include "Nodes/ConcatNode.h"
#include "Nodes/AddNode.h"
#include "Nodes/NumberNode.h"
#include "Nodes/TCPClientNode.h"
#include "Nodes/TCPServerNode.h"
#include "Nodes/TimerNode.h"

int NodeManager::globalId = 100;
using json = nlohmann::json;

NodeManager::NodeManager(std::shared_ptr<TCPServer> tcpServer, std::shared_ptr<TCPClient> tcpClient) 
: tcpServer(tcpServer)
, tcpClient(tcpClient)
{
    ax::NodeEditor::Config config;
    config.NavigateButtonIndex = 2;
    nodeEditorContext = ax::NodeEditor::CreateEditor(&config);
}

ax::NodeEditor::EditorContext* NodeManager::GetEditorContext()
{
    return nodeEditorContext;
}


nlohmann::json NodeManager::Serialize()
{
    nlohmann::json json;
    
    for(std::shared_ptr<Node> node : GetNodes())
    {
        json["nodes"] += node->Serialize();
    }
    
    for(Link link : links)
    {
        json["links"] += link.Serialize();
    }
    
    return json;
}

void NodeManager::SerializeToFile(const std::string& filename)
{
    ax::NodeEditor::SetCurrentEditor(GetEditorContext());

    std::ofstream outputFile(filename);
    if(outputFile.is_open())
    {
        outputFile.clear(); 
        {
            outputFile << Serialize().dump(3);
        }

        outputFile.close();
    }
    ax::NodeEditor::SetCurrentEditor(nullptr);
}

void NodeManager::LoadFromFile(const std::string& filename)
{
    ax::NodeEditor::SetCurrentEditor(GetEditorContext());
    DeleteAllNodes();
    SpawnNodesFromFile(filename);
    ax::NodeEditor::SetCurrentEditor(nullptr);

}

void NodeManager::SpawnNodesFromFile(const std::string& filename)
{

    std::ifstream ifs(filename);

    if(ifs.is_open())
    {
        json json = json::parse(ifs);

        for (auto& [key, val] : json["nodes"].items())
        {
            std::string nodeType = val["type"];
            std::shared_ptr<Node> spawnedNode;
            
            ax::NodeEditor::NodeId id = (uint64_t)val["id"];
            if (nodeType == "ButtonNode")
            {
                spawnedNode =  SpawnNode<ButtonNode>(id);
            }
            if (nodeType == "TimerNode")
            {
                spawnedNode = SpawnNode<TimerNode>(id);
            }
            if (nodeType == "StringNode")
            {
                spawnedNode = SpawnNode<StringNode>(id);
            }
            if (nodeType == "NumberNode")
            {
                spawnedNode = SpawnNode<NumberNode>(id);
            }
            if (nodeType == "ConcatNode")
            {
                spawnedNode = SpawnNode<ConcatNode>(id);
            }
            if (nodeType == "AddNode")
            {
                spawnedNode = SpawnNode<AddNode>(id);
            }
            if (nodeType == "PrintNode")
            {
                spawnedNode = SpawnNode<PrintNode>(id);
            }
            if (nodeType == "TCPClientNode")
            {
                spawnedNode = SpawnNode<TCPClientNode>(id, tcpClient);
            }
            if (nodeType == "TCPServerNode")
            {
                spawnedNode = SpawnNode<TCPServerNode>(id, tcpServer);
            }

            spawnedNode->ConstructFromJSON(val);
            float posX = val["pos_x"];
            float posY = val["pos_y"];
            ax::NodeEditor::SetNodePosition(id, {posX,posY});
        }

        for (auto& [key, val] : json["links"].items())
        {
            NodeManager::globalId++;
            ax::NodeEditor::LinkId id = (uint64_t)val["id"];
            ax::NodeEditor::PinId startId = (uint64_t)val["startPinId"];
            ax::NodeEditor::PinId endId = (uint64_t)val["endPinId"];

            links.emplace_back(Link{id, startId, endId});
        }
//
        ifs.close();
    }
}

void NodeManager::DeleteAllNodes()
{
    for(auto& node : GetNodes())
    ax::NodeEditor::DeleteNode(node->id);
}

void NodeManager::Update()
{
    for(auto& node : nodes)
    {
        node->Update();
    }

    for (auto& linkInfo : links)
    {
        
        std::shared_ptr<Pin> inputPin = GetPinFromId(linkInfo.StartPinID);
        std::shared_ptr<Pin> outputPin = GetPinFromId(linkInfo.EndPinID);
        inputPin->isConnected = true;
        outputPin->isConnected = true;
        
        inputPin->active = outputPin->active;
        inputPin->any = outputPin->any;

        ax::NodeEditor::Link(linkInfo.ID, linkInfo.StartPinID, linkInfo.EndPinID, inputPin->GetColorFromType(inputPin->pinType), 2.0f);
    }

    // Handle creation action, returns true if editor want to create new object (node or link)
    if (ax::NodeEditor::BeginCreate(ImColor(255, 255, 255), 2.0f))
    {
        ax::NodeEditor::PinId inputPinId, outputPinId;
        if (ax::NodeEditor::QueryNewLink(&outputPinId, &inputPinId))
        {
            // QueryNewLink returns true if editor want to create new link between pins.
            //
            // Link can be created only for two valid pins, it is up to you to
            // validate if connection make sense. Editor is happy to make any.
            //
            // Link always goes from input to output. User may choose to drag
            // link from output pin or input pin. This determine which pin ids
            // are valid and which are not:
            //   * input valid, output invalid - user started to drag new ling from input pin
            //   * input invalid, output valid - user started to drag new ling from output pin
            //   * input valid, output valid   - user dragged link over other pin, can be validated

            bool accept = false;
            std::shared_ptr<Pin> inputPin = GetPinFromId(inputPinId);
            std::shared_ptr<Pin> outputPin = GetPinFromId(outputPinId);

            if(inputPin->pinKind == ax::NodeEditor::PinKind::Input  && outputPin->pinKind == ax::NodeEditor::PinKind::Output)
            {
                bool matchesType  = inputPin->pinType == outputPin->pinType || (inputPin->pinType == Pin::PinType::Any && outputPin->pinType != Pin::PinType::Trigger);
                bool pinIsntFull = !inputPin->isConnected;
                if(matchesType && pinIsntFull)
                {
                    accept = true;
                }
            }
            
            if (accept)
            {
                // ed::AcceptNewItem() return true when user release mouse button.
                if (ax::NodeEditor::AcceptNewItem())
                {
                    // Since we accepted new link, lets add one to our list of links.
                    links.push_back({ ax::NodeEditor::LinkId(++NodeManager::globalId), inputPinId, outputPinId});
                    
                    // Draw new link.
                    ax::NodeEditor::Link(links.back().ID, links.back().StartPinID, links.back().EndPinID,inputPin->GetColorFromType(inputPin->pinType), 2.0f);
                }
            }
            else
            {
                ax::NodeEditor::RejectNewItem(ImColor(255, 0, 0), 2.0f);
            }
        }
        ax::NodeEditor::EndCreate(); // Wraps up object creation action handling.
    }

    // Handle deletion action
    if (ax::NodeEditor::BeginDelete())
    {
        ax::NodeEditor::NodeId deletedNodeId;
        while (ax::NodeEditor::QueryDeletedNode(&deletedNodeId))
        {
            // If you agree that link can be deleted, accept deletion.
            if (ax::NodeEditor::AcceptDeletedItem())
            {
                // Then remove link from your data.
                for (auto& node : nodes)
                {
                    if (node->id == deletedNodeId)
                    {
                        nodes.erase(std::remove(nodes.begin(), nodes.end(), node), nodes.end());
                        break;
                    }
                }       
            }

            // You may reject node deletion by calling:
            // ed::RejectDeletedItem();
        }

        // There may be many links marked for deletion, let's loop over them.
        ax::NodeEditor::LinkId deletedLinkId;
        while (ax::NodeEditor::QueryDeletedLink(&deletedLinkId))
        {
            // If you agree that link can be deleted, accept deletion.
            if (ax::NodeEditor::AcceptDeletedItem())
            {
                // Then remove link from your data.
                for (auto& link : links)
                {
                    if (link.ID == deletedLinkId)
                    {
                        std::shared_ptr<Pin> inputPin = GetPinFromId(link.EndPinID);
                        std::shared_ptr<Pin> outputPin = GetPinFromId(link.StartPinID);
                            
                        if(inputPin)
                        {
                            inputPin->isConnected = false;
                            inputPin->any.reset();
                            inputPin->active = false;
                        }
                        if(outputPin)
                        {
                            outputPin->isConnected = false;
                            outputPin->any.reset();
                            outputPin->active = false;
                        }
                        links.erase(std::remove(links.begin(), links.end(), link), links.end());
                        break;
                    }
                }
            }

            // You may reject link deletion by calling:
            // ed::RejectDeletedItem();
        }

        ax::NodeEditor::EndDelete(); // Wrap up deletion action
    }   
}

std::shared_ptr<Pin> NodeManager::GetPinFromId(ax::NodeEditor::PinId pinId)
{
    for (auto& node : nodes)
    {
        for(std::shared_ptr<Pin> pin : node->GetPins())
        {
            if(pin->id == pinId)
            {
                return pin;
            }
        }
    }
    return nullptr;
}