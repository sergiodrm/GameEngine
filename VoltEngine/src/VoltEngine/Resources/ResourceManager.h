#pragma once
#include "VoltEngine/Core/Singleton.h"

namespace Volt
{
    class IResource;

    template <typename T>
    class IResourceManager
    {
        using ResourcePtr = SharedPtr<T>;
    public:
        ResourcePtr Load(const std::string& filepath)
        {
            ResourcePtr resource = Find(filepath);
            if (!resource)
            {
                resource = CreateResource();
                resource->m_name = filepath;
                resource->m_id = m_currentId;
                resource->Load(filepath);
                m_resources[m_currentId] = resource;
                m_resourcesByName[filepath] = m_currentId;
                NextId();
            }
            return resource;
        }

        ResourcePtr Find(const std::string& filepath)
        {
            return m_resourcesByName.count(filepath) > 0 ? Find(filepath) : nullptr;
        }

        ResourcePtr Find(uint32_t id)
        {
            return m_resources.count(id) > 0 ? m_resources[id] : nullptr;
        }

        void UnloadResource(const ResourcePtr& resource)
        {
            UnloadResource(resource->m_name);
        }

        void UnloadResource(const std::string& filepath)
        {
            if (m_resourcesByName.count(filepath) > 0)
            {
                m_resources[m_resourcesByName[filepath]]->Unload();
                m_resources.erase(m_resourcesByName[filepath]);
                m_resourcesByName.erase(filepath);
            }
        }

    protected:
        virtual ResourcePtr CreateResource() = 0;

    private:
        void NextId() { ++m_currentId; }

    protected:
        std::unordered_map<uint32_t, SharedPtr<T>> m_resources;
        std::unordered_map<std::string, uint32_t> m_resourcesByName;
    private:
        uint32_t m_currentId {0};
    };
}
