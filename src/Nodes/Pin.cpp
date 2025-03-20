#include "Nodes/Pin.h"
 #include "Nodes/Drawing.h"
#include "Nodes/Widgets.h"

Pin::Pin(const std::string& name, ax::NodeEditor::PinKind pinKind, PinType pinType)
: id(NodeManager::globalId++)
, pinKind(pinKind)
, name(name)
, pinType(pinType)
{}

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