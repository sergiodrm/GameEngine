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
    Volt::Ref<Volt::CCamera> m_camera;

    Volt::Ref<Volt::CScene> m_scene;

    Volt::Ref<Volt::CEntity> m_cameraEntity;
    Volt::Ref<Volt::CEntity> m_squareEntity;

    Volt::Ref<Volt::ITexture> m_texture;
    Volt::Ref<Volt::ITexture> m_tileSetTexture;
    Volt::Ref<Volt::CSubTexture> m_treeTexture;
    Volt::Ref<Volt::IFramebuffer> m_framebuffer;
    glm::vec2 m_viewportSize;

    glm::vec3 m_position;
};
