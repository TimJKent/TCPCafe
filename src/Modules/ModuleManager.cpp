#include "Modules/ModuleManager.h"
#include <filesystem>
#include <iostream>

void ModuleManager::LoadModules(const std::string& path)
{
    modules.clear();
    std::filesystem::path modulePath(path);
    if(!std::filesystem::exists(modulePath))
    {
        return;
    }

    for (const auto& entry : std::filesystem::directory_iterator(modulePath))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".lua")
        {
            sol::state lua;
            lua.open_libraries(sol::lib::base, sol::lib::package);
            lua.script_file(entry.path().string());

            std::string name = lua["Name"];
            std::string category = lua["Category"];
            std::string description = lua["Description"];
            std::string version = lua["Version"];
            std::string author = lua["Author"];

            modules.emplace_back(name, category, description, version, author, entry.path().string());
        }
    }
}