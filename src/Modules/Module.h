#pragma once

#include <string>
#include <sol/sol.hpp>
#include <memory>

class Module
{
public:
    Module()
    {
        lua.open_libraries(sol::lib::base, sol::lib::package);
    }
  
    void Load(const std::string& path)
    {
        lua.script_file(path);
        name = lua["Name"];
        category = lua["Category"];
        description = lua["Description"];
        version = lua["Version"];
        author = lua["Author"];
        file = path;
    }

    const std::string& GetName() const { return name; }
    const std::string& GetCategory() const { return category; }
    const std::string& GetDescription() const { return description; }
    const std::string& GetVersion() const { return version; }
    const std::string& GetAuthor() const { return author; }
    const std::string& GetFile() const { return file; }
     sol::state& GetLuaState()  { return lua; }
private:
    std::string name;
    std::string category;
    std::string description;
    std::string version;
    std::string author;
    std::string file;
    sol::state lua;
};