#include "Nodes/BooleanOperatorNode.h"
#include "imgui_node_editor.h"
#include "misc/cpp/imgui_stdlib.h"
#include <cmath>

BooleanOperatorNode::BooleanOperatorNode(ax::NodeEditor::NodeId id) : ClonableNode<BooleanOperatorNode>(id)
{
    AddOutputPin("Out", Pin::PinType::Boolean);
    AddInputPin("A", Pin::PinType::Boolean);
    AddInputPin("A", Pin::PinType::Boolean);
}

std::string BooleanOperatorNode::GetNodeTypeName()
{
    return "BooleanOperatorNode";
}

void BooleanOperatorNode::DrawImpl()
{
        ImGui::Text("Bool Op");
        ImGui::SameLine();

        const char* items[] = { "AND", "OR", "NOT", "XOR" };

        ImGui::SetNextItemWidth(80);
        
        if (ImGui::BeginCombo("##OP", items[selected])) {
            for (int n = 0; n < IM_ARRAYSIZE(items); n++) {
                bool is_selected = (selected == n);
                if (ImGui::Selectable(items[n], is_selected)) {
                    selected = n;
                }
                if (is_selected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
}

void BooleanOperatorNode::Update()
{
    if(inputPins[inputPins.size()-1]->isConnected)
    {
        AddInputPin("A", Pin::PinType::Boolean);
    }

    if(inputPins.size() > 2 && !inputPins[inputPins.size()-2]->isConnected)
    {
        RemoveInputPin();
    }

    switch((OPERATOR)selected)
    {
        case AND: outputPins[0]->value = GetOutputForAND(); break;
        case OR:  outputPins[0]->value = GetOutputForOR();  break;
        case NOT: outputPins[0]->value = GetOutputForNOT(); break;
        case XOR: outputPins[0]->value = GetOutputForXOR(); break;
        default:  outputPins[0]->value = false;
    }
}

bool BooleanOperatorNode::GetOutputForAND()
{
    for(std::shared_ptr<Pin> pin : inputPins)
    {
        if(pin->isConnected && !std::get<bool>(pin->value))
        {
            return false;
        }
    }

    return true;
}

bool BooleanOperatorNode::GetOutputForOR()
{
    for(std::shared_ptr<Pin> pin : inputPins)
    {
        if(pin->isConnected && std::get<bool>(pin->value))
        {
            return true;
        }
    }

    return false;
}

bool BooleanOperatorNode::GetOutputForNOT()
{
    bool output = false;

    for(std::shared_ptr<Pin> pin : inputPins)
    {
        if(pin->isConnected)
        {
            return output = !std::get<bool>(pin->value);
        }
    }
    return false;
}

bool BooleanOperatorNode::GetOutputForXOR()
{
    bool output = false;

    for(std::shared_ptr<Pin> pin : inputPins)
    {
        if(pin->isConnected && std::get<bool>(pin->value))
        {
            if(output == true)
            {
                return false;
            }
           output = true;
        }
    }
    return output;
}

void BooleanOperatorNode::SpecialConstructFromJSON(const nlohmann::json& json)
{
    if(json.contains("operator"))
    {
        selected = json["operator"];
    }
}

void BooleanOperatorNode::SpecialSerialze(nlohmann::json& json)
{
    json["operator"] = selected;
}