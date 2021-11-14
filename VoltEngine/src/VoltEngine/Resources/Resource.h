#pragma once
#include <string>


namespace Volt
{
    class IResourceManager;

    class IResource
    {
    public:
        IResource(IResourceManager* creator, const std::string& name, uint32_t id)
            : m_creator(creator), m_name(name), m_id(id) {}

        virtual ~IResource() = default;

        virtual void Load() = 0;
        virtual void Unload() = 0;

        IResourceManager* GetCreator() const { return m_creator; }
        const std::string& GetName() const { return m_name; }
        uint32_t GetId() const { return m_id; }

    private:
        IResourceManager* m_creator;
        std::string m_name;
        uint32_t m_id;
    };
}
