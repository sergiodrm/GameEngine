#include "SceneHierarchyPanel.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "glm/gtc/type_ptr.hpp"
#include "VoltEngine/Renderer/Material.h"
#include "VoltEngine/Renderer/Renderer3D.h"
#include "VoltEngine/Scene/Components/LightComponent.h"
#include "VoltEngine/Scene/Components/NativeScriptComponent.h"


namespace DetailPanelUtils
{
    template <typename T>
    static void DrawComponentHeader(const std::string& name, Volt::CEntity& entity, std::function<void(T&)> drawContentFunction)
    {
        const ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap
            | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth;
        if (entity.HasComponent<T>())
        {
            T* component = entity.GetComponent<T>();
            const ImVec2 regionAvailableSize = ImGui::GetContentRegionAvail();
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.f, 4.f));
            const float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y + 2.f;
            ImGui::Separator();
            const bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), flags, "%s", name.c_str());
            ImGui::PopStyleVar();
            ImGui::SameLine(regionAvailableSize.x - lineHeight * 0.5f);
            if (ImGui::Button("+", ImVec2(lineHeight, lineHeight)))
                ImGui::OpenPopup("Settings");
            bool removed = false;
            if (ImGui::BeginPopup("Settings"))
            {
                removed = ImGui::MenuItem("Remove component");
                ImGui::EndPopup();
            }
            if (open)
            {
                drawContentFunction(*component);
                ImGui::TreePop();
            }
            if (removed)
                entity.RemoveComponent<T>();
        }
    }

    static void DrawFloatControl(const std::string& label, const ImVec4& color, const ImVec4& hoveredColor, float& value, float resetValue = 0.f)
    {
        ImGuiIO& io = ImGui::GetIO();
        ImFont* boldFont = io.Fonts->Fonts[0];
        const float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y + 2.f;
        const ImVec2 buttonSize = {lineHeight + 3.f, lineHeight};
        ImGui::PushStyleColor(ImGuiCol_Button, color);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hoveredColor);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);
        ImGui::PushFont(boldFont);
        if (ImGui::Button(label.c_str(), buttonSize))
            value = resetValue;
        ImGui::PopFont();
        ImGui::PopStyleColor(3);
        ImGui::SameLine();
        static char dragLabel[16];
        memset(dragLabel, 0, sizeof(dragLabel));
        strcpy_s(dragLabel + 2, sizeof(dragLabel), label.c_str());
        dragLabel[0] = '#';
        dragLabel[1] = '#';
        ImGui::DragFloat(dragLabel, &value, 0.1f, 0.f, 0.f, "%.2f");
        ImGui::PopItemWidth();
    }

    static void DrawVec3Control(const std::string& label, glm::vec3& value, float resetValue = 0.f, float columnWidth = 100.f)
    {
        ImGuiIO& io = ImGui::GetIO();
        ImFont* boldFont = io.Fonts->Fonts[0];

        ImGui::PushID(label.c_str());
        ImGui::Columns(2);

        // Left side -> title
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text("%s", label.c_str());
        ImGui::NextColumn();

        // Right side -> content
        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0.f, 0.f});
        DrawFloatControl("X", {0.8f, 0.1f, 0.15f, 1.f}, {0.9f, 0.2f, 0.2f, 1.f}, value.x, resetValue);
        ImGui::SameLine();
        DrawFloatControl("Y", {0.2f, 0.7f, 0.3f, 1.f}, {0.3f, 0.8f, 0.3f, 1.f}, value.y, resetValue);
        ImGui::SameLine();
        DrawFloatControl("Z", {0.1f, 0.25f, 0.8f, 1.f}, {0.2f, 0.2f, 0.9f, 1.f}, value.z, resetValue);
        ImGui::PopStyleVar();
        ImGui::Columns(1);
        ImGui::PopID();
    }
}


namespace Volt
{
    CSceneHierarchyPanel::CSceneHierarchyPanel(const SharedPtr<CScene>& sceneContext)
        : m_sceneContext(sceneContext), m_selection(nullptr) {}

    void CSceneHierarchyPanel::OnUIRender()
    {
        if (m_sceneContext)
        {
            // Scene hierarchy
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

            // Detail panels
            ImGui::Begin("Details panel");
            if (m_selection)
            {
                DrawComponents(*m_selection);
            }
            ImGui::End();

            // Scene globals
            ImGui::Begin("Scene globals");
            if (ImGui::TreeNodeEx("Global"))
            {
                ImGui::Columns(2);
                ImGui::Text("Ambient color");
                ImGui::NextColumn();
                ImGui::ColorEdit3("##ambient color", value_ptr(CRenderer3D::AmbientColor));
                ImGui::NextColumn();
                ImGui::Text("Ambient strength");
                ImGui::NextColumn();
                ImGui::DragFloat("##ambient strength", &CRenderer3D::AmbientStrength, 0.001f, 0.f, 1.f);
                ImGui::NextColumn();
                ImGui::Columns(1);

                ImGui::TreePop();
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

    void CSceneHierarchyPanel::DrawComponents(CEntity& entity)
    {
        CTagComponent* tag = entity.GetComponent<CTagComponent>();
        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        strcpy_s(buffer, sizeof(buffer), tag->GetTag().c_str());
        if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
        {
            tag->SetTag(buffer);
        }

        ImGui::SameLine();
        ImGui::PushItemWidth(-1.f);
        if (ImGui::Button("Add Component"))
        {
            ImGui::OpenPopup("AddComponent");
        }
        if (ImGui::BeginPopup("AddComponent"))
        {
            if (ImGui::MenuItem("Camera Component"))
            {
                m_selection->AddComponent<CCameraComponent>();
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::MenuItem("Sprite Component"))
            {
                m_selection->AddComponent<CSpriteRenderComponent>();
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::MenuItem("Mesh Component"))
            {
                m_selection->AddComponent<CMeshComponent>();
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::MenuItem("Light Component"))
            {
                m_selection->AddComponent<CLightComponent>();
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        ImGui::PopItemWidth();


        DetailPanelUtils::DrawComponentHeader<CTransformComponent>("Transform Component", entity, [](CTransformComponent& component)
        {
            glm::vec3 position = component.GetPosition();
            glm::vec3 rotation = degrees(component.GetRotation());
            glm::vec3 scale = component.GetScale();
            DetailPanelUtils::DrawVec3Control("Position", position);
            DetailPanelUtils::DrawVec3Control("Rotation", rotation);
            DetailPanelUtils::DrawVec3Control("Scale", scale, 1.f);
            component.SetPosition(position);
            component.SetRotation(radians(rotation));
            component.SetScale(scale);
        });
        DetailPanelUtils::DrawComponentHeader<CCameraComponent>("Camera Component", entity, [](CCameraComponent& component)
        {
            ImGui::Columns(2);

            bool primary = component.IsPrimary();
            ImGui::Text("Primary camera");
            ImGui::NextColumn();
            ImGui::Checkbox("##Primary camera", &primary);
            component.SetPrimary(primary);
            ImGui::NextColumn();

            CCamera& camera = component.GetCamera();
            const char* projectionTypeStrings[] = {"Perspective", "Orthographic"};
            const char* currentProjectionTypeString = projectionTypeStrings[static_cast<int32_t>(camera.GetProjectionType())];
            ImGui::Text("Projection type");
            ImGui::NextColumn();
            if (ImGui::BeginCombo("##Projection", currentProjectionTypeString))
            {
                for (int32_t index = 0; index < static_cast<int32_t>(CCamera::EProjectionType::Max); ++index)
                {
                    const bool isSelected = currentProjectionTypeString == projectionTypeStrings[index];
                    if (ImGui::Selectable(projectionTypeStrings[index], isSelected))
                    {
                        currentProjectionTypeString = projectionTypeStrings[index];
                        camera.SetProjectionType(static_cast<CCamera::EProjectionType>(index));
                    }
                    if (isSelected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            ImGui::NextColumn();

            switch (camera.GetProjectionType())
            {
                case CCamera::EProjectionType::Perspective:
                    {
                        float verticalFOV = glm::degrees(camera.GetPerspectiveVerticalFOV());
                        ImGui::Text("Vertical FOV");
                        ImGui::NextColumn();
                        if (ImGui::DragFloat("##Vertical FOV", &verticalFOV))
                            camera.SetPerspectiveVerticalFOV(verticalFOV);
                        ImGui::NextColumn();
                        ImGui::Text("Near clip");
                        ImGui::NextColumn();
                        float nearClip = camera.GetPerspectiveNearClip();
                        if (ImGui::DragFloat("##Near Clip", &nearClip))
                            camera.SetPerspectiveNearClip(nearClip);
                        ImGui::NextColumn();
                        ImGui::Text("Far clip");
                        ImGui::NextColumn();
                        float farClip = camera.GetPerspectiveFarClip();
                        if (ImGui::DragFloat("##Far clip", &farClip))
                            camera.SetPerspectiveFarClip(farClip);
                        ImGui::NextColumn();
                    }
                    break;
                case CCamera::EProjectionType::Orthographic:
                    {
                        float size = camera.GetOrthographicSize();
                        ImGui::Text("Size");
                        ImGui::NextColumn();
                        if (ImGui::DragFloat("##Size", &size))
                            camera.SetOrthographicSize(size);
                        ImGui::NextColumn();
                        ImGui::Text("Near clip");
                        ImGui::NextColumn();
                        float orthoNear = camera.GetOrthographicNearClip();
                        if (ImGui::DragFloat("##Near clip", &orthoNear))
                            camera.SetOrthographicNearClip(orthoNear);
                        ImGui::NextColumn();
                        ImGui::Text("Far clip");
                        ImGui::NextColumn();
                        float orthoFar = camera.GetOrthographicFarClip();
                        if (ImGui::DragFloat("##Far clip", &orthoFar))
                            camera.SetOrthographicFarClip(orthoFar);
                        ImGui::NextColumn();
                    }
                    break;
            }
            ImGui::Columns(1);
        });
        DetailPanelUtils::DrawComponentHeader<CSpriteRenderComponent>("Sprite Render Component", entity, [](CSpriteRenderComponent& component)
        {
            glm::vec4 color = component.GetColor();
            ImGui::Columns(2);

            ImGui::Text("Color");
            ImGui::NextColumn();

            if (ImGui::ColorEdit4("##color", value_ptr(color), ImGuiColorEditFlags_NoInputs))
            {
                component.SetColor(color);
            }
            ImGui::NextColumn();

            ImGui::Text("Texture");
            ImGui::NextColumn();
            const SharedPtr<ITexture>& texture = component.GetTexture();
            std::string newTextureFilePath;
            if (texture)
            {
                void* textureID = reinterpret_cast<void*>(texture->GetRendererID());
                if (ImGui::ImageButton(textureID, {64.f, 64.f}, {0.f, 0.f}, {1.f, -1.f}, 1))
                {
                    newTextureFilePath = CFileDialogs::LoadFile("Images (*.png)\0*.png\0*.jpg\0");
                }
                ImGui::SameLine();

                if (ImGui::Button("Clear"))
                    component.SetTexture(nullptr);
            }
            else
            {
                if (ImGui::Button("Add texture", {64.f, 64.f}))
                {
                    newTextureFilePath = CFileDialogs::LoadFile("Images (*.png)\0*.png\0*.jpg\0");
                }
            }

            if (!newTextureFilePath.empty())
            {
                const SharedPtr<ITexture> newTexture = CAssetManager::LoadAsset<ITexture>(newTextureFilePath);
                if (newTexture)
                {
                    component.SetTexture(newTexture);
                }
            }

            ImGui::Columns(1);
        });
        DetailPanelUtils::DrawComponentHeader<CMeshComponent>("Mesh component", entity, [](CMeshComponent& component)
        {
            const std::string meshName = component.GetMesh() ? component.GetMesh()->GetName() : "Empty";
            const float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y + 2.f;

            if (ImGui::TreeNodeEx("Mesh"))
            {
                ImGui::Columns(2);
                ImGui::Text("Mesh");
                ImGui::NextColumn();
                ImGui::Text("%s", meshName.c_str());
                ImGui::SameLine();
                if (ImGui::Button("...", {lineHeight, lineHeight}))
                {
                    const std::string newMeshPath = CFileDialogs::LoadFile("Mesh (*.obj)\0*.obj\0");
                    if (!newMeshPath.empty())
                    {
                        const SharedPtr<IMesh> newMesh = CAssetManager::LoadAsset<IMesh>(newMeshPath);
                        component.SetMesh(newMesh);
                    }
                }
                ImGui::SameLine();
                if (ImGui::Button("Clear"))
                {
                    component.SetMesh(nullptr);
                }

                ImGui::Columns(1);
                ImGui::TreePop();
            }

            if (ImGui::TreeNodeEx("Material"))
            {
                SharedPtr<IMesh> mesh = component.GetMesh();
                SharedPtr<IMaterial> material = mesh ? mesh->GetMaterial() : nullptr;
                if (material)
                {
                    glm::vec3 ambient = material->GetAmbient();
                    glm::vec3 diffuse = material->GetDiffuse();
                    glm::vec3 specular = material->GetSpecular();
                    float shininess = material->GetShininess();
                    ImGui::Columns(2);
                    ImGui::Text("Ambient");
                    ImGui::NextColumn();
                    if (ImGui::ColorEdit3("##ambient", value_ptr(ambient)))
                    {
                        material->SetAmbient({ambient.x, ambient.y, ambient.z, 1.f});
                    }
                    ImGui::NextColumn();
                    ImGui::Text("Diffuse");
                    ImGui::NextColumn();
                    if (ImGui::ColorEdit3("##diffuse", value_ptr(diffuse)))
                    {
                        material->SetDiffuse({diffuse.x, diffuse.y, diffuse.z, 1.f});
                    }
                    ImGui::NextColumn();
                    ImGui::Text("Specular");
                    ImGui::NextColumn();
                    if (ImGui::ColorEdit3("##specular", value_ptr(specular)))
                    {
                        material->SetSpecular({specular.x, specular.y, specular.z, 1.f});
                    }
                    ImGui::NextColumn();
                    ImGui::Text("Shininess");
                    ImGui::NextColumn();
                    if (ImGui::DragFloat("##shininess", &shininess, 1.f, 0.f, std::numeric_limits<float>::max()))
                    {
                        material->SetShininess(shininess);
                    }
                    ImGui::NextColumn();
                    ImGui::Text("Use texture");
                    ImGui::NextColumn();
                    bool useTexture = material->IsUsingTexture();
                    if (ImGui::Checkbox("##use texture", &useTexture))
                        material->UseTexture(useTexture);
                    ImGui::Columns(1);
                }
                ImGui::TreePop();
            }
        });
        DetailPanelUtils::DrawComponentHeader<CLightComponent>("Light component", entity, [](CLightComponent& component)
        {
            ELightType type = component.GetType();
            glm::vec4 color = component.GetColor();
            ImGui::Columns(2);
            ImGui::Text("Light color");
            ImGui::NextColumn();
            if (ImGui::ColorEdit4("##light color", value_ptr(color)))
            {
                component.SetColor(color);
            }
            ImGui::NextColumn();

            ImGui::Text("Light type");
            ImGui::NextColumn();
            static const char* lightTypes[] = {"Point", "Directional"};
            const char* currentType = lightTypes[static_cast<uint32_t>(type)];
            if (ImGui::BeginCombo("##light type", currentType))
            {
                for (int32_t index = 0; index < static_cast<int32_t>(ELightType::Max); ++index)
                {
                    const bool isSelected = currentType == lightTypes[index];
                    if (ImGui::Selectable(lightTypes[index], isSelected))
                    {
                        currentType = lightTypes[index];
                        component.SetType(static_cast<ELightType>(index));
                    }
                    if (isSelected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            ImGui::Columns(1);
        });
    }
}
