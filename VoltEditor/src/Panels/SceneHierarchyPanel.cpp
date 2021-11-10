#include "SceneHierarchyPanel.h"

#include "imgui.h"

namespace Volt
{
    CSceneHierarchyPanel::CSceneHierarchyPanel(const Ref<CScene>& sceneContext)
        : m_sceneContext(sceneContext), m_selection(nullptr) {}

    void CSceneHierarchyPanel::OnUIRender()
    {
        if (m_sceneContext)
        {
            ImGui::Begin("Scene Hierarchy");
            for (const Ref<CEntity>& it : m_sceneContext->GetRegistry())
            {
                DrawEntity(*it);
            }
            ImGui::End();
        }
    }

    void CSceneHierarchyPanel::DrawEntity(CEntity& entity)
    {
        CTagComponent* tagComponent = entity.GetComponent<CTagComponent>();
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Selected;
        flags |= m_selection && *m_selection == entity ? ImGuiTreeNodeFlags_OpenOnArrow : 0;
        void* id = reinterpret_cast<void*>(entity.GetID());
        const bool opened = ImGui::TreeNodeEx(id, flags, "%s", tagComponent->GetTag().c_str());
        if (ImGui::IsItemClicked())
        {
            m_selection = &entity;
        }
        if (opened)
        {
            ImGui::TreePop();
        }
    }
}
