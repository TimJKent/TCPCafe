#include "Nodes/NodeManager.h"
#include "Nodes/Node.h"

#include <iostream>
#include <algorithm>
#include <fstream>

#include "Nodes/PrintNode.h"
#include "Nodes/PrintNode.h"
#include "Nodes/ButtonNode.h"
#include "Nodes/ToggleNode.h"
#include "Nodes/StringNode.h"
#include "Nodes/ConcatNode.h"
#include "Nodes/AddNode.h"
#include "Nodes/SubtractNode.h"
#include "Nodes/NumberNode.h"
#include "Nodes/TCPClientNode.h"
#include "Nodes/TCPServerNode.h"
#include "Nodes/TimerNode.h"
#include "Nodes/BooleanOperatorNode.h"
#include "Nodes/BooleanDisplayNode.h"

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
    
    for(auto& link : links)
    {
        json["links"] += link->Serialize();
    }
    
    return json;
}

void NodeManager::SetEditorActive(bool active)
{
    if(active)
    {
        ax::NodeEditor::SetCurrentEditor(GetEditorContext());
    }
    else
    {
        ax::NodeEditor::SetCurrentEditor(nullptr);
    }
}

void NodeManager::SerializeToFile(const std::string& filename)
{
    SetEditorActive(true);

    std::ofstream outputFile(filename);
    if(outputFile.is_open())
    {
        outputFile.clear(); 
        {
            outputFile << Serialize().dump(3);
        }

        outputFile.close();
    }
    SetEditorActive(false);
}

void NodeManager::QueueLoadFromFile(const std::string& filename)
{
    queuedImportFilename = filename;
    waitingForDeleteCounter = 2;
}

void NodeManager::SpawnNodesFromFile()
{
    std::ifstream ifs(queuedImportFilename);

    std::unordered_map<uint64_t, uint64_t> idMap;

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
                    spawnedNode =  SpawnNode<ButtonNode>();
                }
                if(nodeType == "BooleanOperatorNode")
                {
                    spawnedNode = SpawnNode<BooleanOperatorNode>();
                }
                if(nodeType == "BooleanDisplayNode")
                {
                    spawnedNode = SpawnNode<BooleanDisplayNode>();
                }
                if (nodeType == "TimerNode")
                {
                    spawnedNode = SpawnNode<TimerNode>();
                }
                if (nodeType == "StringNode")
                {
                    spawnedNode = SpawnNode<StringNode>();
                }
                if (nodeType == "NumberNode")
                {
                    spawnedNode = SpawnNode<NumberNode>();
                }
                if (nodeType == "ConcatNode")
                {
                    spawnedNode = SpawnNode<ConcatNode>();
                }
                if (nodeType == "AddNode")
                {
                    spawnedNode = SpawnNode<AddNode>();
                }
                if (nodeType == "SubtractNode")
                {
                    spawnedNode = SpawnNode<SubtractNode>();
                }
                if (nodeType == "PrintNode")
                {
                    spawnedNode = SpawnNode<PrintNode>();
                }
                if (nodeType == "TCPClientNode")
                {
                    spawnedNode = SpawnNode<TCPClientNode>(tcpClient);
                }
                if (nodeType == "TCPServerNode")
                {
                    spawnedNode = SpawnNode<TCPServerNode>(tcpServer);
                }
                if(nodeType == "ToggleNode")
                {
                    spawnedNode = SpawnNode<ToggleNode>();
                }
            
                spawnedNode->ConstructFromJSON(val, idMap);
                float posX = val["pos_x"];
                float posY = val["pos_y"];
                
                ax::NodeEditor::SetNodePosition(spawnedNode->id, {posX,posY});
            }
            for (auto& [key, val] : json["links"].items())
            {
                ax::NodeEditor::PinId startId = idMap[(uint64_t)val["startPinId"]];
                ax::NodeEditor::PinId endId = idMap[(uint64_t)val["endPinId"]];
            
                links.emplace_back(std::make_unique<Link>(startId, endId));
            }

        ifs.close();
    }
}

void NodeManager::DeleteAllNodes()
{

    for(auto& link : links)
    {
        ax::NodeEditor::DeleteLink(link->ID);
    }

    links.clear();

    
    for(auto& node : GetNodes())
    {
        ax::NodeEditor::DeleteNode(node->id);
    }

    nodes.clear();
}

std::vector<std::shared_ptr<Node>> NodeManager::GetSelectedNodes()
{
    std::vector<ax::NodeEditor::NodeId> selectedNodeIds;
    selectedNodeIds.resize(ax::NodeEditor::GetSelectedObjectCount());

    ax::NodeEditor::GetSelectedNodes(selectedNodeIds.data(), static_cast<int>(selectedNodeIds.size()));

    std::vector<std::shared_ptr<Node>> selectedNodes;

    for(auto& nodeId : selectedNodeIds)
    {
        for(auto& node : nodes)
        {
            if(nodeId == node->id)
            {
                selectedNodes.push_back(node);
            }
        }
    }

    return selectedNodes;
}

void NodeManager::DuplicateSelected()
{
    std::vector<std::shared_ptr<Node>> selectedNodes = GetSelectedNodes();
    UnselectAll();

    for(auto& selectedNode : selectedNodes)
    {
        ImVec2 selectedNodeSize = ax::NodeEditor::GetNodeSize(selectedNode->id);
        ImVec2 selectedNodePos = ax::NodeEditor::GetNodePosition(selectedNode->id);

        std::shared_ptr<Node> spawnedNode = selectedNode->Clone();
        nodes.emplace_back(spawnedNode);
        ax::NodeEditor::SetNodePosition(spawnedNode->id, {selectedNodePos.x,selectedNodePos.y+selectedNodeSize.y});
        ax::NodeEditor::SelectNode(spawnedNode->id, true);
    }
}

void NodeManager::Update()
{
    for(auto& node : nodes)
    {
        node->Update();
    }

    for (auto& linkInfo : links)
    {
        Pin* inputPin = GetPinFromId(linkInfo->StartPinID);
        Pin* outputPin = GetPinFromId(linkInfo->EndPinID);
        inputPin->isConnected = true;
        outputPin->isConnected = true;
        
        inputPin->value = outputPin->value;
        ax::NodeEditor::Link(linkInfo->ID, linkInfo->StartPinID, linkInfo->EndPinID, inputPin->GetColorFromType(), 2.0f);
    }
    // Handle creation action, returns true if editor want to create new object (node or link)
    if (ax::NodeEditor::BeginCreate(ImColor(255, 255, 255), 2.0f))
    {
    ax::NodeEditor::PinId inputPinId, outputPinId;
        if (ax::NodeEditor::QueryNewLink(&outputPinId, &inputPinId))
        {
            bool accept = false;
            Pin* inputPin = GetPinFromId(inputPinId);
            Pin* outputPin = GetPinFromId(outputPinId);

            if(inputPin->pinKind == ax::NodeEditor::PinKind::Input  && outputPin->pinKind == ax::NodeEditor::PinKind::Output)
            {

                bool matchesType  = inputPin->pinType == outputPin->pinType || (inputPin->pinType == Pin::PinType::Any && outputPin->pinType != Pin::PinType::Boolean);
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
                    links.emplace_back(std::make_unique<Link>(inputPinId, outputPinId));
                    
                    // Draw new link.
                    ax::NodeEditor::Link(links.back()->ID, links.back()->StartPinID, links.back()->EndPinID,inputPin->GetColorFromType(), 2.0f);
                }
            }
            else
            {
                ax::NodeEditor::RejectNewItem(ImColor(255, 0, 0), 2.0f);
            }
        }
        ax::NodeEditor::EndCreate(); // Wraps up object creation action handling.
    }

    if(waitingForDeleteCounter == 0 && !queuedImportFilename.empty())
    {
        SpawnNodesFromFile();
    
        queuedImportFilename = "";
    }
    if(waitingForDeleteCounter == 2)
    {
        DeleteAllNodes();
    }

    if(waitingForDeleteCounter > 0)
    {
        waitingForDeleteCounter--;
    }
    
    ProcessQueuedDeletedNodes();   
}

Pin* NodeManager::GetPinFromId(ax::NodeEditor::PinId pinId)
{
    return reinterpret_cast<Pin*>((uintptr_t)pinId);
}

void NodeManager::SelectAll()
{
    for(auto& node : nodes)
    {
        ax::NodeEditor::SelectNode(node->id, true);
    }
}

void NodeManager::UnselectAll()
{
   ax::NodeEditor::ClearSelection();
}

void NodeManager::DoRecenter()
{
    SetEditorActive(true);
    SelectAll();
    ax::NodeEditor::NavigateToSelection(true, 1.0f);
    UnselectAll();
    recenter = false;
    SetEditorActive(false);
}

void NodeManager::ProcessQueuedDeletedNodes()
{
    if (ax::NodeEditor::BeginDelete())
    {
        ax::NodeEditor::NodeId deletedNodeId;

        while (ax::NodeEditor::QueryDeletedNode(&deletedNodeId))
        {
            if (ax::NodeEditor::AcceptDeletedItem())
            {
                for (auto& node : nodes)
                {
                    if (node->id == deletedNodeId)
                    {
                        nodes.erase(std::remove(nodes.begin(), nodes.end(), node), nodes.end());
                        break;
                    }
                }       
            }
        }

        ax::NodeEditor::LinkId deletedLinkId;
        while (ax::NodeEditor::QueryDeletedLink(&deletedLinkId))
        {
            if (ax::NodeEditor::AcceptDeletedItem())
            {
                for (auto& link : links)
                {
                    if (link->ID == deletedLinkId)
                    {
                        Pin* inputPin = GetPinFromId(link->EndPinID);
                        Pin* outputPin = GetPinFromId(link->StartPinID);

                        if(inputPin)
                        {
                            inputPin->isConnected = false;
                        }
                        if(outputPin)
                        {
                            outputPin->isConnected = false;
                        }
                        links.erase(std::remove(links.begin(), links.end(), link), links.end());
                        break;
                    }
                }
            }
        }

        ax::NodeEditor::EndDelete();
    }
}
