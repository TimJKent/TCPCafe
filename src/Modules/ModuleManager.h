#pragma once

#include <sol/sol.hpp>
#include "Modules/Module.h"

class ModuleManager
{
public:
    ModuleManager() = default;
    ~ModuleManager() = default;

    void LoadModules(const std::string& path);
    const std::vector<Module>& GetModules() const { return modules; }
private:
    std::vector<Module> modules;
};