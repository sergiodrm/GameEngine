#pragma once
#include "VoltEngine/Core/Singleton.h"

namespace Volt
{
    class IResource;

    class IResourceManager
    {
    public:
        IResourceManager() = default;
        virtual ~IResourceManager() = default;

        template <typename T>
        Ref<T> CreateResource(const std::string& filepath)
        {
            return Ref<T>(static_cast<T*>(CreateResource(filepath).get()));
        }

        Ref<IResource> CreateResource(const std::string& name);
        void DestroyResource(const Ref<IResource>& resource);
        void DestroyResource(const std::string& name);
        void DestroyResource(uint32_t id);

        Ref<IResource> GetResourceByName(const std::string& name) const;
        Ref<IResource> GetResource(uint32_t resourceID) const;

    protected:
        virtual Ref<IResource> CreateResourceImplementation(uint32_t resourceId, const std::string& filepath) = 0;

    private:
        void NextId();

    protected:
        std::unordered_map<uint32_t, Ref<IResource>> m_resources;
        std::unordered_map<std::string, Ref<IResource>> m_resourcesByName;
    private:
        uint32_t m_currentId {0};
    };
}
