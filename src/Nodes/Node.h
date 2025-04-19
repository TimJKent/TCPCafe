#pragma once

#include "imgui_node_editor.h"
#include <string>
#include <vector>
#include <memory>
#include "Serialization/ISerializable.h"
#include <Nodes/Pin.h>

class Node : public Serialization::ISerializable
{
public:
    Node(ax::NodeEditor::NodeId id = 0);
    Node(const Node& copy);
    virtual ~Node() = default;
public:
    virtual std::string GetNodeTypeName(){return "";}
    virtual std::shared_ptr<Node> Clone() const {return std::make_shared<Node>(*this);}
    virtual void Update(){} 
    ImVec2 GetPosition() const { return ax::NodeEditor::GetNodePosition(id); }
    nlohmann::json Serialize();
    void ConstructFromJSON(const nlohmann::json& json);
    const std::vector<std::shared_ptr<Pin>>& GetInputPins()  const { return inputPins;}
    const std::vector<std::shared_ptr<Pin>>& GetOutputPins() const { return outputPins;}
    void Draw(); 
    void AddInputPin(const std::string& name, Pin::PinType pinType);
    void AddOutputPin(const std::string& name, Pin::PinType pinType);
    void RemoveInputPin();
    void RemoveOutputPin();
protected:
    virtual void SpecialSerialze(nlohmann::json&) {}
    virtual void SpecialConstructFromJSON(const nlohmann::json&){}    
    virtual void DrawImpl(){} 
    void DrawPins();
public:
    ax::NodeEditor::NodeId id;
protected:
    std::vector<std::shared_ptr<Pin>> inputPins;
    std::vector<std::shared_ptr<Pin>> outputPins;
};