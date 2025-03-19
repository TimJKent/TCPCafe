#include "Nodes/Pin.h"

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
        else if(any.type() == typeid(float))
        {
            output = std::to_string(std::any_cast<float>(any));
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

void Pin::Draw()
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