#include "Nodes/PrintNode.h"
#include "imgui_node_editor.h"
#include <iostream>


PrintNode::PrintNode() : Node()
, inputPin("Input", ax::NodeEditor::PinKind::Input)
, outputPin("Output",  ax::NodeEditor::PinKind::Output)
{

}

void PrintNode::Draw()
{
    ax::NodeEditor::BeginNode(id);
        ImGui::Text("Node A");
        inputPin.Draw();
        ImGui::SameLine();
        outputPin.Draw();
    ax::NodeEditor::EndNode();
    
}