#pragma once
#include "VoltEngine/VoltEngine.h"

class CEditorLayer : public Volt::CLayer
{
public:
    virtual void OnAttach() override;
    virtual void OnUpdate(float elapsedSeconds) override;
    virtual void OnUIRender() override;

private:
    Volt::Ref<Volt::CScene> m_scene;

    Volt::Ref<Volt::CEntity> m_cameraEntity;
    Volt::Ref<Volt::CEntity> m_squareEntity;

    Volt::Ref<Volt::IFramebuffer> m_framebuffer;
    glm::vec2 m_viewportSize {0.f};
};
