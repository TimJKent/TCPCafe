#include "Modules/ModuleManager.h"
#include <filesystem>
#include <iostream>
#include <memory>

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
            std::shared_ptr<Module> module = std::make_shared<Module>();
            module->Load(entry.path().string());
            modules[module->GetCategory()].push_back(module);
        }
    }
}