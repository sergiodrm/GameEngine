#include "ResourceManager.h"

#include "Resource.h"

namespace Volt
{
    Ref<IResource> IResourceManager::CreateResource(const std::string& name)
    {
        Ref<IResource> resource = GetResourceByName(name);
        if (!resource)
        {
            resource = CreateResourceImplementation(m_currentId, name);
            if (resource)
            {
                m_resources[m_currentId] = resource;
                m_resourcesByName[name] = resource;
                NextId();
            }
        }
        return resource;
    }

    void IResourceManager::DestroyResource(const Ref<IResource>& resource)
    {
        if (resource)
        {
            m_resources.erase(resource->GetId());
            m_resourcesByName.erase(resource->GetName());
        }
    }

    void IResourceManager::DestroyResource(const std::string& name) { DestroyResource(m_resourcesByName[name]); }
    void IResourceManager::DestroyResource(uint32_t id) { DestroyResource(m_resources[id]); }

    Ref<IResource> IResourceManager::GetResourceByName(const std::string& name) const { return m_resourcesByName.count(name) > 0 ? m_resourcesByName.at(name) : nullptr; }
    Ref<IResource> IResourceManager::GetResource(uint32_t resourceID) const { return m_resources.count(resourceID) > 0 ? m_resources.at(resourceID) : nullptr; }
    void IResourceManager::NextId() { ++m_currentId; }
}
