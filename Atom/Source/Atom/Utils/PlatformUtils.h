#pragma once
#include <filesystem>
#include <string>

#undef SetEnvironmentVariable
#undef GetEnvironmentVariable

namespace Atom
{

    class Regex
    {
    public:
        static bool SetEnvironmentVariable(const std::string& key, const std::string& value);
        static std::string GetEnvironmentVariable(const std::string& key);
    };

    class FileDialogs
    {
    public:
        static std::filesystem::path OpenFolderDialog(const char* initialFolder = "");

        static std::string OpenFile(const char* filter);
        static std::string SaveFile(const char* filter);
    };
    
}