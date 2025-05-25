#pragma once

class Module
{
public:
    Module(const std::string& name,const std::string& category, const std::string& description, const std::string& version, const std::string& author, const std::string& file)
        :  name(name),  category(category), description(description), version(version), author(author), file(file) {}
    const std::string& GetName() const { return name; }
    const std::string& GetCategory() const { return category; }
    const std::string& GetDescription() const { return description; }
    const std::string& GetVersion() const { return version; }
    const std::string& GetAuthor() const { return author; }
    const std::string& GetFile() const { return file; }
private:
    std::string name;
    std::string category;
    std::string description;
    std::string version;
    std::string author;
    std::string file;
};