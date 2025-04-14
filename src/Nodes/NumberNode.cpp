#include "Nodes/NumberNode.h"
#include "imgui_node_editor.h"
#include "misc/cpp/imgui_stdlib.h"
#include <cmath>
#include <iostream>


NumberNode::NumberNode(ax::NodeEditor::NodeId id) : Node(id)
, outputPin(std::make_shared<Pin>("Out", ax::NodeEditor::PinKind::Output,Pin::PinType::Number))
, inputPin1(std::make_shared<Pin>("X", ax::NodeEditor::PinKind::Input,Pin::PinType::Number))
, inputPin2(std::make_shared<Pin>("Set", ax::NodeEditor::PinKind::Input,Pin::PinType::Trigger))
{
}

NumberNode::NumberNode(NumberNode& copy) : Node(++NodeManager::globalId)
, outputPin(std::make_shared<Pin>(*copy.outputPin.get()))
, inputPin1(std::make_shared<Pin>(*copy.inputPin1.get()))
, inputPin2(std::make_shared<Pin>(*copy.inputPin2.get()))
, isFloating(copy.isFloating)
, floatingPoint(copy.floatingPoint)
, integer(copy.integer)
{
}


std::string NumberNode::GetNodeTypeName()
{
    return "NumberNode";
}

void NumberNode::Draw()
{
    ax::NodeEditor::BeginNode(id);
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
        inputPin1->Draw();
        ImGui::SameLine(110);
        outputPin->Draw();
        inputPin2->Draw();
    ax::NodeEditor::EndNode();
}

void NumberNode::Update()
{
    if(inputPin1->active && inputPin2->active)
    {
        if(std::holds_alternative<int>(inputPin1->value))
        {
            integer = std::get<int>(inputPin1->value);
            floatingPoint = (double)integer;
        }
        else if(std::holds_alternative<double>(inputPin1->value))
        {
            floatingPoint = std::get<double>(inputPin1->value);
            integer = (int)std::round(floatingPoint);
        }
    }

    if(isFloating)
    {
        outputPin->value = floatingPoint;
        integer = (int)std::round(floatingPoint);
    }
    else
    {
        outputPin->value = integer;
        floatingPoint = (double)integer;
    }
    outputPin->active = true;
}

std::vector<std::shared_ptr<Pin>> NumberNode::GetPins()
{
    return {inputPin1, inputPin2, outputPin};
}

void NumberNode::ConstructFromJSON(const nlohmann::json& json)
{
    for (auto& [key, val] : json["pins"].items())
    {
        std::shared_ptr<Pin> pin = std::make_shared<Pin>(val);
        if(pin->GetName() == "Out")
        {
            outputPin = pin;
        }else if(pin->GetName() == "X")
        {
            inputPin1 = pin;
        }
        else if(pin->GetName() == "Set")
        {
            inputPin2 = pin;
        }
    }

    isFloating = json["isFloating"];

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

void NumberNode::SpecialSerialze(nlohmann::json& json)
{
    json["isFloating"] = isFloating;
    json["number"] = isFloating ? floatingPoint : integer;
}