#include "Nodes/StringNode.h"
#include "imgui_node_editor.h"
#include "misc/cpp/imgui_stdlib.h"


StringNode::StringNode() : ClonableNode<StringNode>()
{
    AddOutputPin("String", Pin::PinType::Any);
}


std::string StringNode::GetNodeTypeName()
{
    return "StringNode";
}

void StringNode::DrawImpl()
{
        ImGui::Text("String");
        ImGui::SetNextItemWidth(200);
        ImGui::InputText("##String", &string);
        ImGui::SameLine();

}

void StringNode::Update()
{
    outputPins[0]->value = string;
}

void StringNode::SpecialConstructFromJSON(const nlohmann::json& json)
{
    if(json.contains("string"))
    {
        string = json["string"];
    }
}

void StringNode::SpecialSerialze(nlohmann::json& json)
{
    json["string"] = string;
}