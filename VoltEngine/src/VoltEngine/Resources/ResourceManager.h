#pragma once
#include "VoltEngine/Core/Singleton.h"

namespace Volt
{
    class IResource;

    class IResourceManager
    {
    public:
        IResourceManager() = default;
        virtual ~IResourceManager();

        template <typename T>
        T* CreateResource(const std::string& filepath)
        {
            return static_cast<T*>(CreateResource(filepath).get());
        }

        SharedPtr<IResource> CreateResource(const std::string& name);
        void DestroyResource(const SharedPtr<IResource>& resource);
        void DestroyResource(const std::string& name);
        void DestroyResource(uint32_t id);

        SharedPtr<IResource> GetResourceByName(const std::string& name) const;
        SharedPtr<IResource> GetResource(uint32_t resourceID) const;

    protected:
        virtual SharedPtr<IResource> CreateResourceImplementation(uint32_t resourceId, const std::string& filepath) = 0;

    private:
        void NextId();

    protected:
        std::unordered_map<uint32_t, SharedPtr<IResource>> m_resources;
        std::unordered_map<std::string, SharedPtr<IResource>> m_resourcesByName;
    private:
        uint32_t m_currentId {0};
    };
}
