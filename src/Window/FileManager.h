#include <filesystem>

namespace FileManager
{
    bool FileExists(const std::string& pathAndFilename)
    {
        return std::filesystem::exists(pathAndFilename);
    }
}