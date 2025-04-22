#pragma once
#include <filesystem>

namespace FileManager
{
    class Session
    {
        public:
            Session();
            bool IsActive();
        private:
            std::filesystem::path currentPath;
    };

    bool FileExists(const std::string& pathAndFilename);
}