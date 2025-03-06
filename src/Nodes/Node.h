#pragma once

#include "imgui_node_editor.h"
#include <string>
#include <vector>

enum class PinType
{
    Flow,
    Bool,
    Int,
    Float,
    String,
    Object,
    Function,
    Delegate,
};

enum class PinKind
{
    Output,
    Input
};

enum class NodeType
{
    Blueprint,
    Simple,
    Tree,
    Comment,
    Houdini
};

class Node;

class Node
{
public:
    Node();
public:
    ax::NodeEditor::NodeId id;

    virtual void Draw(){}; 
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