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
            m_sceneContext->GetRegistry().Each([this](CEntity* entity)
            {
                DrawEntity(*entity);
            });

            if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsWindowHovered())
            {
                m_selection = nullptr;
            }

            if (ImGui::BeginPopupContextWindow(nullptr, ImGuiMouseButton_Right, false))
            {
                if (ImGui::MenuItem("Create entity"))
                {
                    m_sceneContext->CreateEntity();
                }
                ImGui::EndPopup();
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

        bool deleted = false;
        if (ImGui::BeginPopupContextItem(tagComponent->GetTag().c_str()))
        {
            deleted = ImGui::MenuItem("Delete entity");
            ImGui::EndPopup();
        }

        if (opened)
        {
            ImGui::TreePop();
        }

        if (deleted)
        {
            m_selection = nullptr;
            m_sceneContext->RemoveEntity(entity);
        }
    }
}
