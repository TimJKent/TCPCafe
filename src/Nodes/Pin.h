#pragma once
#include "Nodes/NodeManager.h"
#include <any>

class Pin
{
public:
    enum class PinType
    {
        Trigger,
        Bool,
        Int,
        Float,
        String,
        Object,
        Function,
        Delegate,
        Any,
    };

public:
    Pin(const std::string& name, ax::NodeEditor::PinKind pinKind, PinType pinType)
    : id(NodeManager::globalId++)
    , pinKind(pinKind)
    , name(name)
    , pinType(pinType)
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

    bool active = false;

    ax::NodeEditor::PinId id;
    ax::NodeEditor::PinKind pinKind;
    PinType pinType;
    std::any any;
private:
    std::string name;
    bool triggered = false;
};