#pragma once
#include "EntitiesRegistry.h"
#include "glm/vec2.hpp"
#include "VoltEngine/Core/Core.h"

namespace Volt
{
    class CEntity;

    class CScene
    {
    public:
        CEntity* CreateEntity(const std::string& name = "EmptyEntity");
        void RemoveEntity(CEntity& entity);

        void OnUpdate(float elapsedSeconds);
        void OnViewportResize(uint32_t width, uint32_t height);

        CEntity* GetPrimaryCamera() const;

        glm::vec2 GetViewportSize() const { return {static_cast<float>(m_width), static_cast<float>(m_height)}; }
        uint32_t GetViewportWidth() const { return m_width; }
        uint32_t GetViewportHeight() const { return m_height; }

        const CEntitiesRegistry& GetRegistry() const { return m_registry; }
        CEntitiesRegistry& GetRegistry() { return m_registry; }

    protected:
        void RunEntitiesScripts(float elapsedSeconds);
        void RenderScene();

    private:
        CEntitiesRegistry m_registry;

        uint32_t m_width {0};
        uint32_t m_height {0};
    };
}
