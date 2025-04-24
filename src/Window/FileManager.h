#pragma once
#include <filesystem>
#include <vector>
#include <string>

namespace FileManager
{
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
            void AddRecentFile(const std::string& path);
            std::vector<std::string> GetRecentFiles();
        public:
            bool AppDataPathInitilized = false;
        private:
            std::filesystem::path currentPath;
            std::filesystem::path tcpCafeAppDataPath;
    };

    bool FileExists(const std::string& pathAndFilename);
    std::filesystem::path GetAppDataPath();
}
