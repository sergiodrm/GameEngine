#pragma once
#include "VoltEngine/VoltEngine.h"

namespace Volt
{
    class CLogPanel;
    class CGizmo;
    class CStatsPanel;
    class CSceneHierarchyPanel;
}

class CEditorLayer : public Volt::CLayer
{
public:
    virtual void OnAttach() override;
    virtual void OnUpdate(float elapsedSeconds) override;
    virtual void OnUIRender() override;
    virtual bool OnEvent(Volt::CEvent& e) override;

protected:
    void NewScene();
    void SaveScene();
    void LoadScene();

    bool OnPressMouseButton(Volt::CMouseButtonPressedEvent& e);

private:
    bool m_viewportFocused {true};

    Volt::SharedPtr<Volt::CScene> m_scene;
    Volt::SharedPtr<Volt::CSceneHierarchyPanel> m_sceneHierarchyPanel;
    Volt::SharedPtr<Volt::CStatsPanel> m_statsPanel;
    Volt::SharedPtr<Volt::CLogPanel> m_logPanel;
    Volt::SharedPtr<Volt::CGizmo> m_gizmo;


    Volt::SharedPtr<Volt::IFramebuffer> m_framebuffer;
    Volt::SharedPtr<Volt::CEditorCamera> m_editorCamera;
    glm::vec2 m_viewportSize {0.f};
    glm::vec2 m_viewportBounds[2];
    int32_t m_selectedEntityHandle {-1};
};
