#include "Nodes/Pin.h"
 #include "Nodes/Drawing.h"
#include "Nodes/Widgets.h"
#include "Nodes/NodeManager.h"
#include <iostream>


using json = nlohmann::json;

Pin::Pin(const std::string& name, ax::NodeEditor::PinKind pinKind, PinType pinType)
: id(reinterpret_cast<uint64_t>(this))
, pinKind(pinKind)
, pinType(pinType)
, value(false)
, name(name)
{
    value = false;
}

Pin::Pin(json json, std::unordered_map<uint64_t, uint64_t>& idMap)
: id(reinterpret_cast<uint64_t>(this))
, pinKind(ax::NodeEditor::PinKind::Input)
, pinType(PinType::Any)
, value(false)
, name("")
{
    
    idMap[(uint64_t)json["id"]] = (uint64_t)id;
    pinKind = json["flow"] == 0 ? ax::NodeEditor::PinKind::Input : ax::NodeEditor::PinKind::Output;
    pinType = json["type"];
    name = json["name"];
}

Pin::Pin(Pin& copy)
: id(reinterpret_cast<uint64_t>(this))
, pinKind(copy.pinKind)
, pinType(copy.pinType)
, value(copy.value)
, name(copy.name)
{

}

std::string Pin::PinOutputToString()
{
    std::string output = ""; 

    if(std::holds_alternative<std::string>(value))
    {
        output = std::get<std::string>(value);
    }
    else if(std::holds_alternative<bool>(value))
    {
        output = std::get<bool>(value) ? "true" : "false";
    }
    else if(std::holds_alternative<double>(value))
    {
      output = std::to_string(std::get<double>(value));
    }
    else if(std::holds_alternative<int>(value))
    {
        output = std::to_string(std::get<int>(value));
    }
    

    return output;
}

ImColor Pin::GetColorFromType()
{
    switch(pinType)
    {
        case PinType::Any: return ImColor{204, 191, 139};
        case PinType::Number: return ImColor{91, 151, 207};
        case PinType::Boolean: if(std::holds_alternative<bool>(value) && std::get<bool>(value)){return ImColor{145, 255, 168};}else{return ImColor{75, 173, 108};}
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
    ax::Widgets::Icon(ImVec2(static_cast<float>(24), static_cast<float>(24)), ax::Drawing::IconType::Circle, isConnected, GetColorFromType(), {0,0,0,0});
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