#include "Nodes/LuaNode.h"
#include "imgui_node_editor.h"
#include "misc/cpp/imgui_stdlib.h"
#include <cmath>
#include <sol/sol.hpp>
#include <iostream>


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


    env.set_function("GetPinValue", [this](const std::string& name) {
        for(auto& pin : inputPins)
        {
            if(pin->GetName() == name)
            {
                if(std::holds_alternative<std::string>(pin->value))
                {
                    return sol::make_object(env.lua_state(), pin->value);
                }
                else if(std::holds_alternative<int>(pin->value))
                {
                    return sol::make_object(env.lua_state(), pin->value);
                }
                else if(std::holds_alternative<double>(pin->value))
                {
                    return sol::make_object(env.lua_state(), pin->value);
                }
                else if(std::holds_alternative<bool>(pin->value))
                {
                    return sol::make_object(env.lua_state(), pin->value);
                }
            }
        }
        return sol::make_object(env.lua_state(), sol::nil);
    });

    env.set_function("SetPinValue", [this](const std::string& name, sol::object obj) {
        for(auto& pin : outputPins)
        {
            if(pin->GetName() != name)
            {
                continue;
            }

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