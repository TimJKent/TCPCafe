#pragma once

#include "imgui_node_editor.h"
#include "Nodes/Node.h"
#include <memory>
#include <vector>

class TCPClient;

class NodeManager
{
public:
    static int globalId;
public:
    NodeManager();
public:
    template <typename T, typename... Args>
    std::shared_ptr<Node> SpawnNode(Args... args)
    {
        std::shared_ptr<Node> node = std::make_shared<T>(args...);
        nodes.emplace_back(node);
        return node;
    }
    void SpawnClientSendNode(std::shared_ptr<TCPClient> tcpClient);
    ax::NodeEditor::EditorContext* GetEditorContext();
    std::vector<std::shared_ptr<Node>>& GetNodes() {return nodes;}
    void Update();
    std::shared_ptr<Pin> GetPinFromId(ax::NodeEditor::PinId pinId);
private:
    ax::NodeEditor::EditorContext* nodeEditorContext = nullptr;
    std::vector<std::shared_ptr<Node>> nodes;
    std::vector<Link> links; 
};