#pragma once

#include "VoltEngine/VoltEngine.h"

namespace Volt
{
    class CSceneHierarchyPanel
    {
    public:
        CSceneHierarchyPanel(const SharedPtr<CScene>& sceneContext);

        void OnUIRender();

        CEntity* GetSelection() const { return m_selection; }

    protected:
        void DrawEntity(CEntity& entity);
        void DrawComponents(CEntity& entity);

    private:
        SharedPtr<CScene> m_sceneContext;
        CEntity* m_selection;
    };
}
