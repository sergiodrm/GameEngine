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
            VOLT_LOG(Trace, "Resource [{0}, {1}] destroyed.", m_id, m_name.c_str());
#endif
        }

        virtual void Load(const std::string& filepath) = 0;
        virtual void Unload() = 0;

        const std::string& GetName() const { return m_name; }
        uint32_t GetId() const { return m_id; }

    private:
        std::string m_name;
        uint32_t m_id;
    };
}
