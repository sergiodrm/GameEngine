#pragma once

#include "VoltEngine/VoltEngine.h"

namespace Volt
{
    class CSceneHierarchyPanel
    {
    public:
        CSceneHierarchyPanel(const Ref<CScene>& sceneContext);

        void OnUIRender();

    protected:
        void DrawEntity(CEntity& entity);
        void DrawComponents(CEntity& entity);

    private:
        Ref<CScene> m_sceneContext;
        CEntity* m_selection;
    };
}
