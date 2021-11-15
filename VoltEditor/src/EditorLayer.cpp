#include "EditorLayer.h"

#include "imgui.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/StatsPanel.h"
#include "VoltEngine/Renderer/TextureManager.h"
#include "VoltEngine/Renderer/VertexData.h"
#include "VoltEngine/Scene/Components/NativeScriptComponent.h"

#define TEST_TEXTURE_PATH "assets/textures/sample.png"


static Volt::SharedPtr<Volt::IMesh> CreateCube()
{
    static Volt::SharedPtr<Volt::IMesh> mesh {nullptr};
    if (mesh == nullptr)
    {
        const float offset = -0.5f;
        const std::vector<Volt::SVertexData> vertices =
            {
                // bottom face
                {(glm::vec3(0.f, 0.f, 0.f) + offset), glm::vec4(1.f)},
                {(glm::vec3(0.f, 1.f, 0.f) + offset), glm::vec4(1.f)},
                {(glm::vec3(1.f, 1.f, 0.f) + offset), glm::vec4(1.f)},
                {(glm::vec3(1.f, 0.f, 0.f) + offset), glm::vec4(1.f)},
                // top face                                                
                {(glm::vec3(0.f, 0.f, 1.f) + offset), glm::vec4(1.f)},
                {(glm::vec3(0.f, 1.f, 1.f) + offset), glm::vec4(1.f)},
                {(glm::vec3(1.f, 1.f, 1.f) + offset), glm::vec4(1.f)},
                {(glm::vec3(1.f, 0.f, 1.f) + offset), glm::vec4(1.f)},
                // side face 1                              
                {(glm::vec3(1.f, 1.f, 0.f) + offset), glm::vec4(1.f)},
                {(glm::vec3(0.f, 1.f, 1.f) + offset), glm::vec4(1.f)},
                {(glm::vec3(1.f, 1.f, 1.f) + offset), glm::vec4(1.f)},
                {(glm::vec3(0.f, 1.f, 0.f) + offset), glm::vec4(1.f)},
                // side face 2                              
                {(glm::vec3(1.f, 0.f, 0.f) + offset), glm::vec4(1.f)},
                {(glm::vec3(1.f, 1.f, 0.f) + offset), glm::vec4(1.f)},
                {(glm::vec3(1.f, 1.f, 1.f) + offset), glm::vec4(1.f)},
                {(glm::vec3(1.f, 0.f, 1.f) + offset), glm::vec4(1.f)},
                // side face 3                              
                {(glm::vec3(0.f, 0.f, 0.f) + offset), glm::vec4(1.f)},
                {(glm::vec3(1.f, 0.f, 0.f) + offset), glm::vec4(1.f)},
                {(glm::vec3(1.f, 0.f, 1.f) + offset), glm::vec4(1.f)},
                {(glm::vec3(0.f, 0.f, 1.f) + offset), glm::vec4(1.f)},
                // side face 4                              
                {(glm::vec3(0.f, 1.f, 0.f) + offset), glm::vec4(1.f)},
                {(glm::vec3(0.f, 0.f, 0.f) + offset), glm::vec4(1.f)},
                {(glm::vec3(0.f, 0.f, 1.f) + offset), glm::vec4(1.f)},
                {(glm::vec3(0.f, 1.f, 1.f) + offset), glm::vec4(1.f)}
            };
        std::vector<uint32_t> indices = {
                // bottom
                0, 1, 2,
                0, 2, 3,

                // top
                4, 6, 5,
                4, 7, 6,

                8, 11, 9,
                8, 9, 10,

                12, 13, 14,
                12, 14, 15,

                16, 17, 18,
                16, 18, 19,

                20, 21, 22,
                20, 22, 23
            };
        mesh = Volt::IMesh::Create(vertices, indices);
    }
    return mesh;
}


void CEditorLayer::OnAttach()
{
    CLayer::OnAttach();

    m_framebuffer = Volt::IFramebuffer::Create({1280, 720});

    m_scene = Volt::CreateSharedPtr<Volt::CScene>();

    m_cameraEntity = m_scene->CreateEntity("CameraEntity");
    Volt::CCameraComponent* cameraComponent = m_cameraEntity->AddComponent<Volt::CCameraComponent>(true);
    Volt::CTransformComponent* transformComponent = m_cameraEntity->GetComponent<Volt::CTransformComponent>();
    transformComponent->SetPosition({0.f, 1.7f, 5.f});
    transformComponent->SetRotation(radians(glm::vec3 {-18.3f, 0.f, 0.f}));

    Volt::SharedPtr<Volt::IMesh> cubeMesh = CreateCube();

    Volt::CEntity* cubeEntity = m_scene->CreateEntity("Cube");
    cubeEntity->AddComponent<Volt::CMeshComponent>(cubeMesh);

    m_sceneHierarchyPanel = Volt::CreateSharedPtr<Volt::CSceneHierarchyPanel>(m_scene);
    m_statsPanel = Volt::CreateSharedPtr<Volt::CStatsPanel>();

    Volt::CRenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.f});
}

void CEditorLayer::OnUpdate(float elapsedSeconds)
{
    CLayer::OnUpdate(elapsedSeconds);
    //PROFILE_SCOPE(GameLayer);

    const Volt::SFramebufferSpecification& spec = m_framebuffer->GetSpecification();
    if (m_viewportSize.x > 0.f && m_viewportSize.y > 0.f &&
        (spec.Width != static_cast<uint32_t>(m_viewportSize.x) || spec.Height != static_cast<uint32_t>(m_viewportSize.y)))
    {
        m_framebuffer->Resize(m_viewportSize);
        m_scene->OnViewportResize(spec.Width, spec.Height);
    }
    {
        //PROFILE_SCOPE(Render);

        m_framebuffer->Bind();
        Volt::CRenderCommand::Clear();
        m_scene->OnUpdate(elapsedSeconds);
        m_framebuffer->Unbind();
    }
}

void CEditorLayer::OnUIRender()
{
    m_sceneHierarchyPanel->OnUIRender();
    m_statsPanel->OnUIRender();
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.f, 0.f});
        ImGui::Begin("Viewport");

        const ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        if (glm::vec2(viewportPanelSize.x, viewportPanelSize.y) != m_viewportSize && viewportPanelSize.x > 0 && viewportPanelSize.y > 0)
        {
            m_viewportSize = {viewportPanelSize.x, viewportPanelSize.y};
        }

        const uint32_t texID = m_framebuffer->GetColorAttachmentRendererID();
        ImGui::Image(reinterpret_cast<void*>(texID), viewportPanelSize, {0.f, 1.f}, {1.f, 0.f});
        ImGui::End();
        ImGui::PopStyleVar();
    }
}
