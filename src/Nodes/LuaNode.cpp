#include "Nodes/LuaNode.h"
#include "imgui_node_editor.h"
#include "misc/cpp/imgui_stdlib.h"
#include <cmath>
#include <sol/sol.hpp>


LuaNode::LuaNode(std::shared_ptr<Module> module) : ClonableNode<LuaNode>()
, module(module)
{
    env = sol::environment(module->GetLuaState(), sol::create, module->GetLuaState().globals());
    module->GetLuaState().safe_script_file(module->GetFile(), env);

    env.new_enum("PinType",
        "Any", Pin::PinType::Any,
        "Bool", Pin::PinType::Boolean,
        "Number", Pin::PinType::Number
    );

    env.set_function("AddInputPin", [this](const std::string& name, Pin::PinType pinType) {
       return AddInputPin(name, pinType);
    });

    env.set_function("AddOutputPin", [this](const std::string& name, Pin::PinType pinType) {
       return AddOutputPin(name, pinType);
    });

    env.set_function("GetPinValueAsDouble", [this](const std::string& name) {
        for(auto& pin : inputPins)
        {
            if(pin->GetName() == name)
            {
                if(std::holds_alternative<double>(pin->value))
                {
                    return std::get<double>(pin->value);
                }

            }
        }
        return 0.0;
    });

    env.set_function("GetPinValueAsInt", [this](const std::string& name) {
        for(auto& pin : inputPins)
        {
            if(pin->GetName() == name)
            {
                if(std::holds_alternative<int>(pin->value))
                {
                    return std::get<int>(pin->value);
                }
            }
        }
        return 0;
    });

    env.set_function("GetPinValueAsString", [this](const std::string& name) {
        for(auto& pin : inputPins)
        {
            if(pin->GetName() == name)
            {
                if(std::holds_alternative<std::string>(pin->value))
                {
                    return std::get<std::string>(pin->value);
                }
                else if(std::holds_alternative<int>(pin->value))
                {
                    return std::to_string(std::get<int>(pin->value));
                }
                else if(std::holds_alternative<double>(pin->value))
                {
                    return std::to_string(std::get<double>(pin->value));
                }
            }
        }
        return std::string("");
    });

    env.set_function("GetPinValueAsBool", [this](const std::string& name) {
        for(auto& pin : inputPins)
        {
            if(pin->GetName() == name)
            {
                if(std::holds_alternative<bool>(pin->value))
                {
                    return std::get<bool>(pin->value);
                }
            }
        }
        return false;
    });

    env.set_function("SetPinValue", [this](const std::string& name, sol::object obj) {
        for(auto& pin : inputPins)
        {
           switch (obj.get_type()) {
            case sol::type::number:
                pin->value = obj.as<double>();
                break;
            case sol::type::string:
                pin->value = obj.as<std::string>();
                break;
            case sol::type::boolean:
                pin->value = obj.as<bool>();
                break;
           }
        }
    });

    startFunc = env["Start"];
    updateFunc = env["Update"];

    if(startFunc.valid())
    {
        startFunc();
    }
}

std::string LuaNode::GetNodeTypeName()
{
    return module->GetName();
}

void LuaNode::DrawImpl()
{
    ImGui::Text(GetNodeTypeName().c_str());
}

void LuaNode::Update()
{
   if(startFunc.valid())
    {
        updateFunc();
    }
}