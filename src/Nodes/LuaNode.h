#pragma once
#include "Nodes/ClonableNode.h"
#include "Modules/Module.h"


class LuaNode : public ClonableNode<LuaNode>
{
public:
    LuaNode(std::shared_ptr<Module> module);
    void DrawImpl() final;
    void Update() final;
    std::string GetNodeTypeName() final;
private:
    std::shared_ptr<Module> module;
    sol::environment env;
    sol::function startFunc;
    sol::function updateFunc;  
};