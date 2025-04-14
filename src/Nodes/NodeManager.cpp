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
#include "Nodes/SubtractNode.h"
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
                    if (nodeType == "SubtractNode")
                    {
                        spawnedNode = SpawnNode<SubtractNode>(id);
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

        ifs.close();
    }
}

void NodeManager::DeleteAllNodes()
{

    for(auto& link : links)
    {
        ax::NodeEditor::DeleteLink(link.ID);
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

    int nodeCount = ax::NodeEditor::GetSelectedNodes(selectedNodeIds.data(), static_cast<int>(selectedNodeIds.size()));


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

        std::shared_ptr<Node> spawnedNode = SpawnNode(selectedNode);
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
        }

        ax::NodeEditor::EndDelete();
    }
}
