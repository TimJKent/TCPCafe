#include "Window/FileManager.h"

#include <cstdlib>
#include <iostream>
#include <fstream>
#include "nlohmann/json.hpp"

FileManager::Session::Session()
: currentPath("")
{
    std::filesystem::path appDataPath = GetAppDataPath();
    if(!appDataPath.empty())
    {
        tcpCafeAppDataPath = appDataPath / "TCPCafe";

        if(!FileExists(tcpCafeAppDataPath.string()))
        {
            std::filesystem::create_directory(tcpCafeAppDataPath);
        }

        std::filesystem::path recentFilesPath = tcpCafeAppDataPath / "Recent.json";
        if(!FileExists(recentFilesPath.string()))
        {
            std::ofstream recentFilesFile(recentFilesPath);

            recentFilesFile.close();
        }
        AppDataPathInitilized = true;
    }
}

void FileManager::Session::AddRecentFile(const std::string& path)
{
    if(!AppDataPathInitilized){return;}
    std::filesystem::path recentFilesPath = tcpCafeAppDataPath / "Recent.json";
    std::ifstream ifs(recentFilesPath);
    nlohmann::json json;
    if(ifs.is_open())
    {
        try
        {
            json = nlohmann::json::parse(ifs);
        }
        catch(const nlohmann::json::parse_error& )
        {
        }
        std::vector<RecentFile> recentFiles;

        if(json.contains("RecentFiles"))
        {
            for(auto& recentFile : json["RecentFiles"])
            {
                if(recentFile.contains("order") && recentFile.contains("path"))
                {
                    recentFiles.push_back({recentFile["order"], recentFile["path"]}); 
                }
            }
        }

        int fileOrder = -1;
        for(RecentFile& recentFile : recentFiles)
        {
            if(recentFile.path == path)
            {
                fileOrder = recentFile.order;
                recentFile.order = 0;
            }
        }

        if(fileOrder >= 0)
        {
            for(auto& recentFile : recentFiles)
            {
                if(recentFile.order <= fileOrder)
                {
                    recentFile.order++;
                }
            }       
        }else
        {
            recentFiles.push_back({0, path});
            for(auto& recentFile : recentFiles)
            {
                recentFile.order++;
            } 
        }

        recentFiles.erase(std::remove_if(recentFiles.begin(), recentFiles.end(), [](const RecentFile& recentFile) {
            if(!FileExists(recentFile.path)){return true;}
            return recentFile.order > 10;
        }), recentFiles.end());

        json["RecentFiles"] = nlohmann::json::array();
        for(auto& recentFile : recentFiles)
        {
            nlohmann::json recentFileJSON;
            recentFileJSON["order"] = recentFile.order;
            recentFileJSON["path"] = recentFile.path;
            json["RecentFiles"].push_back(recentFileJSON);
        }
        
        ifs.close();
    }
    std::ofstream ofs(recentFilesPath);
    if(ofs.is_open())
    {
        ofs << json.dump(4);
        ofs.close();
    }
}

std::vector<std::string> FileManager::Session::GetRecentFiles()
{
    if(!AppDataPathInitilized){return {};}
    std::filesystem::path recentFilesPath = tcpCafeAppDataPath / "Recent.json";
    std::ifstream ifs(recentFilesPath);
    nlohmann::json json;
    if(ifs.is_open())
    {
        try
        {
            json = nlohmann::json::parse(ifs);
        }
        catch(const nlohmann::json::parse_error& )
        {
        }
        std::vector<RecentFile> recentFiles;

        if(json.contains("RecentFiles"))
        {
            for(auto& recentFile : json["RecentFiles"])
            {
                if(recentFile.contains("order") && recentFile.contains("path"))
                {
                    recentFiles.push_back({recentFile["order"], recentFile["path"]}); 
                }
            }
        }

        recentFiles.erase(std::remove_if(recentFiles.begin(), recentFiles.end(), [](const RecentFile& recentFile) {
            return !FileExists(recentFile.path);
        }), recentFiles.end());

        std::sort(recentFiles.begin(), recentFiles.end(), [](const RecentFile& a, const RecentFile& b) {
            return a.order < b.order;
        });

        std::vector<std::string> recentPaths;

        for(auto& recentFile : recentFiles)
        {
            recentPaths.push_back(recentFile.path);
        }

        return recentPaths;
    }
    
    return {};
}

bool FileManager::Session::IsActive()
{
    return !currentPath.empty();
}

void FileManager::Session::SetActivePath(const std::string& newActivePath)
{
    currentPath = newActivePath;
}

std::string FileManager::Session::GetActiveFileName()
{
    return currentPath.filename().string();
}

std::string FileManager::Session::GetActivePath()
{
    return currentPath.remove_filename().string();
}

std::filesystem::path appDataPath;


std::string FileManager::Session::GetActivePathAndFileName()
{
    return currentPath.string();
}


bool FileManager::FileExists(const std::string& pathAndFilename)
{
    return std::filesystem::exists(pathAndFilename);
}

std::filesystem::path FileManager::GetAppDataPath()
{
    const char* appDataPath = std::getenv("APPDATA");
    if (appDataPath != nullptr)
    {
        return std::filesystem::path(appDataPath);
    } 
    else 
    {
        return "";
    }
}

