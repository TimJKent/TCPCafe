#pragma once

#include "imgui_node_editor.h"
#include "Nodes/Node.h"
#include <memory>
#include <vector>

class NodeManager
{
public:
    static int globalId;
public:
    NodeManager();
public:
    void SpawnInputActionNode();
    void SpawnButtonNode();
    void SpawnStringNode();
    void SpawnConcatNode();
    ax::NodeEditor::EditorContext* GetEditorContext();
    std::vector<std::shared_ptr<Node>>& GetNodes() {return nodes;}
    void Update();
    std::shared_ptr<Pin> GetPinFromId(ax::NodeEditor::PinId pinId);
private:
    ax::NodeEditor::EditorContext* nodeEditorContext = nullptr;
    std::vector<std::shared_ptr<Node>> nodes;
    std::vector<Link> links; 
};