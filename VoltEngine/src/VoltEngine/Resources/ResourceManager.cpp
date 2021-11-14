#include "ResourceManager.h"

#include "Resource.h"

namespace Volt
{
    IResourceManager::~IResourceManager()
    {
        for (const auto& it : m_resources)
        {
            it.second->Unload();
#ifdef VOLT_DEBUG
            if (it.second.use_count() > 2)
            {
                VOLT_LOG(Error, "There are {0} uses of resource [{1}, {2}], but the manager will be destroyed.", it.second.use_count(), it.first, it.second->GetName().c_str());
            }
#endif
        }
        m_resources.clear();
        m_resourcesByName.clear();
    }

    SharedPtr<IResource> IResourceManager::CreateResource(const std::string& name)
    {
        SharedPtr<IResource> resource = GetResourceByName(name);
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

    void IResourceManager::DestroyResource(const SharedPtr<IResource>& resource)
    {
        if (resource)
        {
            m_resources.erase(resource->GetId());
            m_resourcesByName.erase(resource->GetName());
        }
    }

    void IResourceManager::DestroyResource(const std::string& name) { DestroyResource(m_resourcesByName[name]); }
    void IResourceManager::DestroyResource(uint32_t id) { DestroyResource(m_resources[id]); }

    SharedPtr<IResource> IResourceManager::GetResourceByName(const std::string& name) const { return m_resourcesByName.count(name) > 0 ? m_resourcesByName.at(name) : nullptr; }
    SharedPtr<IResource> IResourceManager::GetResource(uint32_t resourceID) const { return m_resources.count(resourceID) > 0 ? m_resources.at(resourceID) : nullptr; }
    void IResourceManager::NextId() { ++m_currentId; }
}
