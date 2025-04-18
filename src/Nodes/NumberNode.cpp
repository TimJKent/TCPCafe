#include "Nodes/NumberNode.h"
#include "imgui_node_editor.h"
#include "misc/cpp/imgui_stdlib.h"
#include <cmath>
#include <iostream>


NumberNode::NumberNode(ax::NodeEditor::NodeId id) : Node(id)
{
    AddOutputPin("Out", Pin::PinType::Number);
    AddInputPin("X", Pin::PinType::Number);
    AddInputPin("Set", Pin::PinType::Boolean);
}


std::string NumberNode::GetNodeTypeName()
{
    return "NumberNode";
}

void NumberNode::DrawImpl()
{
        ImGui::Text("Number");
        ImGui::SameLine();
        if( ImGui::RadioButton("Decimal", isFloating)){isFloating = !isFloating;}
        ImGui::SetNextItemWidth(150);
        if(isFloating)
        {
            ImGui::InputDouble("##double", &floatingPoint);
        }
        else
        {
            ImGui::InputInt("##Int", &integer);
        }
}

void NumberNode::Update()
{
    if(inputPins[0]->isConnected && inputPins[1]->isConnected && std::get<bool>(inputPins[1]->value))
    {
        if(std::holds_alternative<int>(inputPins[0]->value))
        {
            integer = std::get<int>(inputPins[0]->value);
            floatingPoint = (double)integer;
        }
        else if(std::holds_alternative<double>(inputPins[0]->value))
        {
            floatingPoint = std::get<double>(inputPins[0]->value);
            integer = (int)std::round(floatingPoint);
        }
    }

    if(isFloating)
    {
        outputPins[0]->value = floatingPoint;
        integer = (int)std::round(floatingPoint);
    }
    else
    {
        outputPins[0]->value = integer;
        floatingPoint = (double)integer;
    }
}


void NumberNode::SpecialConstructFromJSON(const nlohmann::json& json)
{
    if(json.contains("isFloating"))
    {
        isFloating = json["isFloating"];
    }

    if(json.contains("number"))
    {
        if(isFloating)
        {
            floatingPoint = json["number"];
            integer = (int)std::round(floatingPoint);
        }
        else
        {
            integer = json["number"];
            floatingPoint = (double)integer;
        }
    }
}

void NumberNode::SpecialSerialze(nlohmann::json& json)
{
    json["isFloating"] = isFloating;
    json["number"] = isFloating ? floatingPoint : integer;
}