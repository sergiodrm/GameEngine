#pragma once
#include "VoltEngine/VoltEngine.h"

namespace Volt
{
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

private:
    Volt::SharedPtr<Volt::CScene> m_scene;
    Volt::SharedPtr<Volt::CSceneHierarchyPanel> m_sceneHierarchyPanel;
    Volt::SharedPtr<Volt::CStatsPanel> m_statsPanel;

    Volt::CEntity* m_cameraEntity {nullptr};
    Volt::CEntity* m_squareEntity {nullptr};

    Volt::SharedPtr<Volt::IFramebuffer> m_framebuffer;
    Volt::SharedPtr<Volt::CEditorCamera> m_editorCamera;
    glm::vec2 m_viewportSize {0.f};
};
