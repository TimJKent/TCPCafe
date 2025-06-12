#pragma once
#include <filesystem>
#include <vector>
#include <string>
#include "Modules/ModuleManager.h"

namespace FileManager
{
    void OpenFileExplorerAtPath(const std::string& path);

    class Session
    {
        public:
            struct RecentFile
            {
                int order = 0;
                std::string path = "";
            };

            Session();
            bool IsActive();
            void SetActivePath(const std::string& newActivePath);
            std::string GetActiveFileName();
            std::string GetActivePath();
            std::string GetActivePathAndFileName();
            std::string GetModulePath();
            void AddRecentFile(const std::string& path);
            std::vector<std::string> GetRecentFiles();
            void ReloadModules();
        public:
            bool AppDataPathInitilized = false;            
            std::filesystem::path currentPath;
            std::filesystem::path tcpCafeAppDataPath;
            const char* appDataFolderName = "TCPCafe";
            const char* moduleFolderName = "Modules";
            const char* recentsFileName = "Recent.json";
            ModuleManager moduleManager;
    };

    bool FileExists(const std::string& pathAndFilename);
    std::filesystem::path GetAppDataPath();
}
