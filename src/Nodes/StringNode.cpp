#include "Nodes/StringNode.h"
#include "imgui_node_editor.h"
#include "misc/cpp/imgui_stdlib.h"


StringNode::StringNode(ax::NodeEditor::NodeId id) : Node(id)
, outputPin(std::make_shared<Pin>("String", ax::NodeEditor::PinKind::Output, Pin::PinType::Any))
{

}

std::string StringNode::GetNodeTypeName()
{
    return "StringNode";
}

void StringNode::Draw()
{
    ax::NodeEditor::BeginNode(id);
        ImGui::Text("String");
        ImGui::SetNextItemWidth(200);
        ImGui::InputText("##String", &string);
        ImGui::SameLine();
        outputPin->Draw();
    ax::NodeEditor::EndNode();

    outputPin->any = std::make_any<std::string>(string);
    outputPin->active = true;
}

std::vector<std::shared_ptr<Pin>> StringNode::GetPins()
{
    return {outputPin};
}

void StringNode::ConstructFromJSON(const nlohmann::json& json)
{
    for (auto& [key, val] : json["pins"].items())
    {
        std::shared_ptr<Pin> pin = std::make_shared<Pin>(val);
        if(pin->GetName() == "String")
        {
            outputPin = pin;
        }
    }

    string = json["string"];
}


void StringNode::SpecialSerialze(nlohmann::json& json)
{
    json["string"] = string;
}