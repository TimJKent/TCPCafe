#pragma once

#include "imgui_node_editor.h"
#include "Serialization/ISerializable.h"

class Link : public Serialization::ISerializable
{
public:
    Link(ax::NodeEditor::LinkId id, ax::NodeEditor::PinId startPinId, ax::NodeEditor::PinId endPinId)
    : ID(id)
    , StartPinID(startPinId)
    , EndPinID(endPinId)
    , Color(255, 255, 255)
    {}
public:
    nlohmann::json Serialize() final 
    {
        nlohmann::json json;
        json["id"]         = ID.Get();
        json["startPinId"] = StartPinID.Get();
        json["endPinId"]   = EndPinID.Get();
        return json;
    }
public:
    bool operator==(const Link& rhs)
    {
        return rhs.ID == ID;
    }
public:
    ax::NodeEditor::LinkId ID;
    ax::NodeEditor::PinId StartPinID;
    ax::NodeEditor::PinId EndPinID;
    ImColor Color;
};