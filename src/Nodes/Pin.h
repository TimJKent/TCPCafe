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
        Number,
        Float,
        Object,
        Function,
        Delegate,
        Any,
    };
public:
    Pin(const std::string& name, ax::NodeEditor::PinKind pinKind, PinType pinType);
    void Draw();
    std::string PinOutputToString();
public:
    bool active = false;
    ax::NodeEditor::PinId id;
    ax::NodeEditor::PinKind pinKind;
    PinType pinType;
    std::any any;
private:
    std::string name;
    bool triggered = false;
};