#pragma once
#include "Nodes/NodeManager.h"
#include <any>
#include <variant>

class Pin : public Serialization::ISerializable
{
public:
    enum class PinType
    {
        Boolean,
        Number,
        Any,
    };
public:
    nlohmann::json Serialize() final;
public:
    ImColor GetColorFromType();

    Pin(const std::string& name, ax::NodeEditor::PinKind pinKind, PinType pinType);
    Pin(Pin& pin);
    Pin(nlohmann::json json);
    void Draw();
    std::string PinOutputToString();
    std::string GetName(){return name;}
public:
    bool isConnected = false;
    ax::NodeEditor::PinId id;
    ax::NodeEditor::PinKind pinKind;
    PinType pinType;
    std::variant<std::string, bool, double, int> value;
private:
    std::string name;
    bool triggered = false;
};