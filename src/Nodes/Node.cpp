#include "Nodes/Node.h"
#include "Nodes/NodeManager.h"
#include <Nodes/Pin.h>



Node::Node(ax::NodeEditor::NodeId nodeID)
: id(nodeID)
{
    ++NodeManager::globalId;
    if((uint64_t)id== 0){id = NodeManager::globalId;}
}

nlohmann::json Node::Serialize()
{
    nlohmann::json json;

    json["id"] = (uint64_t)id;
    json["type"] = GetNodeTypeName();
    ImVec2 position =  GetPosition();
    json["pos_x"] = position.x;
    json["pos_y"] = position.y;
    for(std::shared_ptr<Pin> pin : GetPins())
    {
        json["pins"] += pin->Serialize();
    }
    
    SpecialSerialze(json);

    return json;
}