#pragma once

#include <sol/sol.hpp>
#include "Modules/Module.h"
#include <unordered_map>
#include <vector>
#include <memory>

class ModuleManager
{
public:
    ModuleManager() = default;
    ~ModuleManager() = default;

    void LoadModules(const std::string& path);
    const std::unordered_map<std::string, std::vector<std::shared_ptr<Module>>>& GetModules() const { return modules; }
private:
    std::unordered_map<std::string, std::vector<std::shared_ptr<Module>>> modules;
};