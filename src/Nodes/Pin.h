#pragma once
#include "Nodes/NodeManager.h"
#include <any>

class Pin
{
public:
    enum class PinType
    {
        Trigger,
        Number,
        Any,
    };
public:

    ImColor GetColorFromType(PinType type);

    Pin(const std::string& name, ax::NodeEditor::PinKind pinKind, PinType pinType);
    void Draw();
    std::string PinOutputToString();
public:
    bool active = false;
    bool isConnected = false;
    ax::NodeEditor::PinId id;
    ax::NodeEditor::PinKind pinKind;
    PinType pinType;
    std::any any;
private:
    std::string name;
    bool triggered = false;
};