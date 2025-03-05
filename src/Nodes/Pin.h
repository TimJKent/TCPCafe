#pragma once
#include "Nodes/Node.h"

class Pin
{
public:
    Pin(const std::string& name, ax::NodeEditor::PinKind pinKind)
    : id(Node::globalId++)
    , pinKind(pinKind)
    , name(name)
    {}

    void Draw()
    {
        if(pinKind == ax::NodeEditor::PinKind::Output)
        {
            ImGui::Text(name.c_str());
            ImGui::SameLine();
        }
        ax::NodeEditor::BeginPin(id, pinKind);
            ImGui::Text(" * ");
        ax::NodeEditor::EndPin();
        if(pinKind == ax::NodeEditor::PinKind::Input)
        {
            ImGui::SameLine();
            ImGui::Text(name.c_str());
        }
    }

private:
    int id;
    ax::NodeEditor::PinKind pinKind;
    std::string name;
};