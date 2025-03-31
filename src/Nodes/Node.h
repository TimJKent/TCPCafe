#pragma once

#include "imgui_node_editor.h"
#include <string>
#include <vector>
#include <memory>
#include "Serialization/ISerializable.h"

class Pin;

class Node : public Serialization::ISerializable
{
public:
    Node(ax::NodeEditor::NodeId id = 0);
public:
    nlohmann::json Serialize();
    virtual std::shared_ptr<Node> Clone(){return std::make_shared<Node>(*this);}
    virtual void SpecialSerialze(nlohmann::json& json) {}
    virtual void ConstructFromJSON(const nlohmann::json& json){}
    virtual std::vector<std::shared_ptr<Pin>> GetPins() {return {};}
    ImVec2 GetPosition()
    {
        return ax::NodeEditor::GetNodePosition(id);
    }
    virtual std::string GetNodeTypeName(){return "";}
    virtual void Draw(){} 
    virtual void Update(){} 
public:
    ax::NodeEditor::NodeId id;
};