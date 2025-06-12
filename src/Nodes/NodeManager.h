#pragma once

#include "imgui_node_editor.h"
#include "Nodes/Node.h"
#include "Nodes/Link.h"
#include <memory>
#include <vector>
#include <nlohmann/json.hpp>
#include "Serialization/ISerializable.h"
#include <TCP/Client/TCPClient.h>
#include <TCP/Server/TCPServer.h>
#include "Window/FileManager.h"

class TCPClient;

class NodeManager : public Serialization::ISerializable
{
public:
    NodeManager(std::shared_ptr<TCPServer> tcpServer, std::shared_ptr<TCPClient> tcpClient, FileManager::Session& session);
public:
    void DeleteAllNodes();
    void QueueLoadFromFile(const std::string& filename);
    void SelectAll();
    void UnselectAll();
    void DoRecenter();
    std::vector<std::shared_ptr<Node>> GetSelectedNodes();
    void DuplicateSelected();
    void SetEditorActive(bool active);
    nlohmann::json Serialize();
    template <typename T, typename... Args>
    std::shared_ptr<Node> SpawnNode(Args... args)
    {
        std::shared_ptr<Node> node = std::make_shared<T>(args...);
        nodes.emplace_back(node);
        return node;
    }

    std::shared_ptr<Node> SpawnNode(std::shared_ptr<Node> node)
    {
        std::shared_ptr<Node> cloneNode = node->Clone();
        nodes.emplace_back(cloneNode);
        return cloneNode;
    }
    void SpawnClientSendNode(std::shared_ptr<TCPClient> tcpClient);
    ax::NodeEditor::EditorContext* GetEditorContext();
    std::vector<std::shared_ptr<Node>>& GetNodes() {return nodes;}
    void Update();
    Pin* GetPinFromId(ax::NodeEditor::PinId pinId);
    void SerializeToFile(const std::string& filename);
private:
    void ProcessQueuedDeletedNodes();
    void SpawnNodesFromFile();
private:
    std::shared_ptr<TCPServer> tcpServer;
    std::shared_ptr<TCPClient> tcpClient;
    ax::NodeEditor::EditorContext* nodeEditorContext = nullptr;
    std::vector<std::shared_ptr<Node>> nodes;
    std::vector<std::unique_ptr<Link>> links; 
    bool deleteAll = false;
    bool recenter = false;
    int waitingForDeleteCounter = 0;
    std::string queuedImportFilename = "";
    FileManager::Session& session;
};