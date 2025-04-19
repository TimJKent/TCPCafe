#include "Nodes/AddNode.h"
#include "imgui_node_editor.h"
#include "misc/cpp/imgui_stdlib.h"
#include <cmath>

AddNode::AddNode(ax::NodeEditor::NodeId id) : ClonableNode<AddNode>(id)
{
    AddOutputPin("Sum", Pin::PinType::Number);
    AddInputPin("A", Pin::PinType::Number);
    AddInputPin("A", Pin::PinType::Number);
}

std::string AddNode::GetNodeTypeName()
{
    return "AddNode";
}

void AddNode::DrawImpl()
{
    ImGui::Text("Add");
}

void AddNode::Update()
{
   //int iOutput = 0;
   //double fOutput = 0.0f;
   //
   //bool outputAsFloat = false;

   //for(std::shared_ptr<Pin> pin : inputPins)
   //{
   //    if(std::holds_alternative<int>(pin->value))
   //    {
   //        iOutput += std::get<int>(pin->value);
   //        fOutput += std::get<int>(pin->value);
   //    }
   //    if(std::holds_alternative<double>(pin->value))
   //    {
   //        iOutput += (int)std::round(std::get<double>(pin->value));
   //        fOutput += std::get<double>(pin->value);
   //        outputAsFloat = true;
   //    }
   //}

   //outputPins[0]->value =  outputAsFloat ? fOutput : iOutput;

   //if(inputPins[inputPins.size()-1]->isConnected)
   //{
   //    AddInputPin("A", Pin::PinType::Number);
   //}

   //if(inputPins.size() > 2 && !inputPins[inputPins.size()-2]->isConnected)
   //{
   //    RemoveInputPin();
   //}
}