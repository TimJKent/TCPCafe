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
    void SpawnNode(Args... args)
    {
        nodes.emplace_back(std::make_shared<T>(args...));
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