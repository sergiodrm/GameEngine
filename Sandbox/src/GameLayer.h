#pragma once

#include "VoltEngine/VoltEngine.h"

class CGameLayer : public Volt::CLayer
{
public:
    CGameLayer(const std::string& name = "GameLayer");

    virtual void OnAttach() override;
    virtual void OnUpdate(float elapsedSeconds) override;

private:
    Volt::Ref<Volt::CCamera> m_camera;

    Volt::Ref<Volt::IShader> m_shader;
    Volt::Ref<Volt::IVertexArray> m_vertexArray;
    Volt::Ref<Volt::ITexture> m_texture;

    glm::vec3 m_position;
};
