#pragma once

#include "imgui_node_editor.h"
#include <string>
#include <vector>
#include <memory>

class Pin;

class Node
{
public:
    Node();
public:
    ax::NodeEditor::NodeId id;
    virtual std::vector<std::shared_ptr<Pin>> GetPins() {return {};}
    virtual void Draw(){} 
    virtual void Update(){} 
};

class Link
{
public:
    ax::NodeEditor::LinkId ID;

    ax::NodeEditor::PinId StartPinID;
    ax::NodeEditor::PinId EndPinID;

    ImColor Color;

    bool operator==(const Link& rhs)
    {
        return rhs.ID == ID;
    }

    Link(ax::NodeEditor::LinkId id, ax::NodeEditor::PinId startPinId, ax::NodeEditor::PinId endPinId):
        ID(id), StartPinID(startPinId), EndPinID(endPinId), Color(255, 255, 255)
    {
    }
};