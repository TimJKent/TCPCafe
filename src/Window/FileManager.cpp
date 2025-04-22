#pragma once
#include "Window/FileManager.h"

FileManager::Session::Session()
: currentPath("")
{
}

bool FileManager::Session::IsActive()
{
    return !currentPath.empty();
}

bool FileManager::FileExists(const std::string& pathAndFilename)
{
    return std::filesystem::exists(pathAndFilename);
}