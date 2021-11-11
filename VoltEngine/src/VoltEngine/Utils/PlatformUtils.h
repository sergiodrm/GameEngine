#pragma once
#include <string>


namespace Volt
{
    class CFileDialogs
    {
    public:
        static std::string LoadFile(const char* filter);
        static std::string SaveFile(const char* filter);
    };
}
