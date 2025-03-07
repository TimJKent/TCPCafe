#include "Nodes/ButtonNode.h"
#include "imgui_node_editor.h"
#include "misc/cpp/imgui_stdlib.h"


ButtonNode::ButtonNode() : Node()
, outputPin(std::make_shared<Pin>("Trigger", ax::NodeEditor::PinKind::Output, Pin::PinType::Trigger))
{

}

void ButtonNode::Draw()
{
    ax::NodeEditor::BeginNode(id);
        ImGui::Text("Button");
        if(ImGui::Button("Trigger"))
        {
            outputPin->active = true;
        }
        else
        {
            outputPin->active = false;
        }
        outputPin->Draw();
    ax::NodeEditor::EndNode();
    
}

std::vector<std::shared_ptr<Pin>> ButtonNode::GetPins()
{
    return {outputPin};
}