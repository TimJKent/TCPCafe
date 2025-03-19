#include "Nodes/NumberNode.h"
#include "imgui_node_editor.h"
#include "misc/cpp/imgui_stdlib.h"
#include <cmath>

NumberNode::NumberNode() : Node()
, outputPin(std::make_shared<Pin>("Out(1)", ax::NodeEditor::PinKind::Output,Pin::PinType::Number))
, inputPin1(std::make_shared<Pin>("X(1)", ax::NodeEditor::PinKind::Input,Pin::PinType::Number))
, inputPin2(std::make_shared<Pin>("T", ax::NodeEditor::PinKind::Input,Pin::PinType::Trigger))
{

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
            ImGui::InputFloat("##Float", &floatingPoint);
        }
        else
        {
            ImGui::InputInt("##Int", &integer);
        }
        inputPin1->Draw();
        ImGui::SameLine();
        outputPin->Draw();
        inputPin2->Draw();
    ax::NodeEditor::EndNode();
}

void NumberNode::Update()
{
    if(inputPin1->active && inputPin2->active && inputPin1->any.has_value())
    {
        if(inputPin1->any.type() == typeid(int))
        {
            integer = std::any_cast<int>(inputPin1->any);
            floatingPoint = (float)integer;
        }
        else if(inputPin1->any.type() == typeid(float))
        {
            floatingPoint = std::any_cast<float>(inputPin1->any);
            integer = (int)std::round(floatingPoint);
        }
    }

    if(isFloating)
    {
        outputPin->any = std::make_any<float>(floatingPoint);
        integer = (int)std::round(floatingPoint);
    }
    else
    {
        outputPin->any = std::make_any<int>(integer);
        floatingPoint = (float)integer;
    }
    outputPin->active = true;
}

std::vector<std::shared_ptr<Pin>> NumberNode::GetPins()
{
    return {inputPin1, inputPin2, outputPin};
}