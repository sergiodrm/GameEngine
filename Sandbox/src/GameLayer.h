#pragma once

#include "VoltEngine/VoltEngine.h"


class CGameLayer : public Volt::CLayer
{
public:
    CGameLayer(const std::string& name = "GameLayer");

    virtual void OnAttach() override;
    virtual void OnUpdate(float elapsedSeconds) override;
    virtual void OnUIRender() override;

private:
    Volt::SharedPtr<Volt::CScene> m_scene;

    Volt::SharedPtr<Volt::CEntity> m_cameraEntity;
    Volt::SharedPtr<Volt::CEntity> m_squareEntity;

    Volt::SharedPtr<Volt::IFramebuffer> m_framebuffer;
    glm::vec2 m_viewportSize;
};
