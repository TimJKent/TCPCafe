#pragma once

#include "imgui_node_editor.h"
#include "Nodes/Node.h"
#include <memory>

class NodeManager
{
public:
    static int globalId;
public:
    NodeManager();
public:
    void SpawnInputActionNode();
    ax::NodeEditor::EditorContext* GetEditorContext();
    std::vector<std::shared_ptr<Node>>& GetNodes() {return nodes;}
    void Update();
private:
    ax::NodeEditor::EditorContext* nodeEditorContext = nullptr;
    std::vector<std::shared_ptr<Node>> nodes;
    std::vector<Link> links; 
};