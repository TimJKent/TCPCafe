#include "Nodes/Node.h"
#include "Nodes/NodeManager.h"

Node::Node(ax::NodeEditor::NodeId nodeID)
: id(nodeID)
{
    ++NodeManager::globalId;
    if((uint64_t)id.AsPointer() == 0){id = NodeManager::globalId;}
}

Node::Node(const Node& copy)
: id(++NodeManager::globalId)
{
    for(auto& pin : copy.inputPins)
    {
        inputPins.push_back(std::make_shared<Pin>(*pin.get()));
    }
    for(auto& pin : copy.outputPins)
    {
        outputPins.push_back(std::make_shared<Pin>(*pin.get()));
    }
}

void Node::Draw()
{
    ax::NodeEditor::BeginNode(id);
    DrawImpl();
    DrawPins();
    ax::NodeEditor::EndNode();
}

void Node::DrawPins()
{
    size_t index = 0;
    for(; index < inputPins.size(); index++)
    {
        inputPins[index]->Draw();
        if(index < outputPins.size())
        {
            ImGui::SameLine(100);
            outputPins[index]->Draw();
        }
    }
    for(; index < outputPins.size(); index++)
    {
        outputPins[index]->Draw();
    }
}

nlohmann::json Node::Serialize()
{
    nlohmann::json json;

    json["id"] = (uint64_t)id.AsPointer();
    json["type"] = GetNodeTypeName();
    ImVec2 position =  GetPosition();
    json["pos_x"] = position.x;
    json["pos_y"] = position.y;
    for(std::shared_ptr<Pin> pin : GetInputPins())
    {
        json["pins"] += pin->Serialize();
    }
    for(std::shared_ptr<Pin> pin : GetOutputPins())
    {
        json["pins"] += pin->Serialize();
    }
    
    SpecialSerialze(json);

    return json;
}

void Node::ConstructFromJSON(const nlohmann::json& json)
{
    inputPins.clear();
    outputPins.clear();
    for (auto& [key, val] : json["pins"].items())
    {
        std::shared_ptr<Pin> pin = std::make_shared<Pin>(val);
        if(pin->pinKind == ax::NodeEditor::PinKind::Input)
        {
            inputPins.emplace_back(pin);
        }else
        {
            outputPins.emplace_back(pin);
        }
    }
    SpecialConstructFromJSON(json);
}

void Node::AddInputPin(const std::string& name, Pin::PinType pinType)
{
    inputPins.emplace_back(std::make_shared<Pin>(name, ax::NodeEditor::PinKind::Input, pinType));
}

void Node::RemoveInputPin() 
{ 
    inputPins.pop_back(); 
}

void Node::AddOutputPin(const std::string& name, Pin::PinType pinType)
{
    outputPins.emplace_back(std::make_shared<Pin>(name, ax::NodeEditor::PinKind::Output, pinType));
}

void Node::RemoveOutputPin() 
{ 
    outputPins.pop_back(); 
}