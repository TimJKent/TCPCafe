#include "Nodes/Pin.h"
 #include "Nodes/Drawing.h"
#include "Nodes/Widgets.h"
#include <iostream>


using json = nlohmann::json;

Pin::Pin(const std::string& name, ax::NodeEditor::PinKind pinKind, PinType pinType)
: id(++NodeManager::globalId)
, pinKind(pinKind)
, name(name)
, pinType(pinType)
{
    value = 0.0f;
}

Pin::Pin(json json)
: id(0)
, pinKind(ax::NodeEditor::PinKind::Input)
, name("")
, pinType(PinType::Any)
{
    ++NodeManager::globalId;
    id = (uint64_t)json["id"];
    pinKind = json["flow"] == 0 ? ax::NodeEditor::PinKind::Input : ax::NodeEditor::PinKind::Output;
    pinType = json["type"];
    name = json["name"];
}

Pin::Pin(Pin& copy)
: id(++NodeManager::globalId)
, pinKind(copy.pinKind)
, name(copy.name)
, pinType(copy.pinType)
, value(copy.value)
{

}

std::string Pin::PinOutputToString()
{
    std::string output = "";
    
    if(any.has_value())
    {
        if(any.type() == typeid(std::string))
        {
            output = std::any_cast<std::string>(any);
        }
        else if(any.type() == typeid(int))
        {
            output = std::to_string(std::any_cast<int>(any));
        }
        else if(any.type() == typeid(double))
        {
            output = std::to_string(std::any_cast<double>(any));
        }
        else if(any.type() == typeid(double))
        {
            output = std::to_string(std::any_cast<double>(any));
        }
        else if(any.type() == typeid(bool))
        {
            bool value = std::any_cast<bool>(any);
            output = value ? "true" : "false";
        }
    }

    return output;
}

ImColor Pin::GetColorFromType(PinType type)
{
    switch(type)
    {
        case PinType::Any: return ImColor{204, 191, 139};
        case PinType::Number: return ImColor{91, 151, 207};
        case PinType::Trigger: return ImColor{75, 173, 108};
    }
    return ImColor{255,255,255};
}

void Pin::Draw()
{
    if(pinKind == ax::NodeEditor::PinKind::Output)
    {
        ImGui::Text(name.c_str());
        ImGui::SameLine();
    }
    ax::NodeEditor::BeginPin(id, pinKind);
    ax::Widgets::Icon(ImVec2(static_cast<float>(24), static_cast<float>(24)), ax::Drawing::IconType::Circle, isConnected, GetColorFromType(pinType), {0,0,0,0});
    ax::NodeEditor::EndPin();
    if(pinKind == ax::NodeEditor::PinKind::Input)
    {
        ImGui::SameLine();
        ImGui::Text(name.c_str());
    }
}

json Pin::Serialize() 
{
    json pinJson;

    pinJson["id"] = id.Get();
    pinJson["name"] = name;
    pinJson["type"] = pinType;  
    pinJson["flow"] = pinKind;  
    std::visit([&pinJson](auto &&arg) -> void {
        pinJson["value"] = arg;
    }, value);

    return pinJson;
}