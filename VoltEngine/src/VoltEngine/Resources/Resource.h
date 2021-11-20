#pragma once
#include <string>

#include "ResourceManager.h"


namespace Volt
{
    class IResource
    {
        friend IResourceManager;
    public:
        virtual ~IResource()
        {
#ifdef VOLT_DEBUG
            VOLT_LOG(Trace, "Resource [{0}, {1}] destroyed.", m_handle, m_name.c_str());
#endif
        }

        virtual void Load() = 0;
        virtual void Unload() = 0;

        const std::string& GetName() const { return m_name; }
        const std::string& GetFilepath() const { return m_filepath; }
        uint32_t GetHandle() const { return m_handle; }

    private:
        std::string m_name;
        std::string m_filepath;
        uint32_t m_handle;
    };
}
