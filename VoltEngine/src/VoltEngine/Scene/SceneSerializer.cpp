#include "SceneSerializer.h"

#include <filesystem>
#include <fstream>

#include "Scene.h"
#include "Components/CameraComponent.h"
#include "Components/LightComponent.h"
#include "Components/MeshComponent.h"
#include "Components/NativeScriptComponent.h"
#include "Components/SpriteRenderComponent.h"
#include "Components/TransformComponent.h"
#include "glm/glm.hpp"
#include "VoltEngine/AssetManager/AssetManager.h"
#include "VoltEngine/Core/Log.h"
#include "VoltEngine/Core/Time.h"
#include "VoltEngine/Renderer/Material.h"
#include "VoltEngine/Renderer/Mesh.h"
#include "VoltEngine/Renderer/Renderer3D.h"
#include "VoltEngine/Renderer/Texture.h"
#include "yaml-cpp/yaml.h"

namespace YAML
{
    template <>
    struct convert<glm::vec3>
    {
        static Node encode(const glm::vec3& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            return node;
        }

        static bool decode(const Node& node, glm::vec3& rhs)
        {
            if (!node.IsSequence() || node.size() != 3)
            {
                return false;
            }
            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            return true;
        }
    };

    template <>
    struct convert<glm::vec4>
    {
        static Node encode(const glm::vec4& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.push_back(rhs.w);
            return node;
        }

        static bool decode(const Node& node, glm::vec4& rhs)
        {
            if (!node.IsSequence() || node.size() != 4)
            {
                return false;
            }
            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            rhs.w = node[3].as<float>();
            return true;
        }
    };
}

namespace Volt
{
    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& value)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << value.x << value.y << value.z << YAML::EndSeq;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& value)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << value.x << value.y << value.z << value.w << YAML::EndSeq;
        return out;
    }

    static void SerializeEntity(YAML::Emitter& out, CEntity* entity)
    {
        PROFILE_SCOPE(EntitySerialization);
        out << YAML::BeginMap; // Begin entity
        out << YAML::Key << "EntityHandle" << YAML::Value << entity->GetID();

        if (entity->HasComponent<CTagComponent>())
        {
            CTagComponent* tag = entity->GetComponent<CTagComponent>();
            VOLT_LOG(Trace, "Serializing entity {0}...", tag->GetTag().c_str());

            out << YAML::Key << "TagComponent";
            out << YAML::BeginMap; // Begin TagComponent
            out << YAML::Key << "Tag" << YAML::Value << tag->GetTag().c_str();
            out << YAML::EndMap; // End TagComponent
        }
        if (entity->HasComponent<CTransformComponent>())
        {
            CTransformComponent* transform = entity->GetComponent<CTransformComponent>();
            out << YAML::Key << "TransformComponent";
            out << YAML::BeginMap; // Begin TransformComponent
            out << YAML::Key << "Translation" << YAML::Value << transform->GetPosition();
            out << YAML::Key << "Rotation" << YAML::Value << transform->GetRotation();
            out << YAML::Key << "Scale" << YAML::Value << transform->GetScale();
            out << YAML::EndMap; // End TransformComponent
        }
        if (entity->HasComponent<CCameraComponent>())
        {
            CCameraComponent* camera = entity->GetComponent<CCameraComponent>();
            out << YAML::Key << "CameraComponent";
            out << YAML::BeginMap; // Begin CameraComponent
            out << YAML::Key << "Primary" << YAML::Value << camera->IsPrimary();
            out << YAML::EndMap; // End CameraComponent
        }
        if (entity->HasComponent<CLightComponent>())
        {
            CLightComponent* light = entity->GetComponent<CLightComponent>();
            out << YAML::Key << "LightComponent";
            out << YAML::BeginMap; // Begin LightComponent
            out << YAML::Key << "Type" << YAML::Value << static_cast<uint32_t>(light->GetType());
            out << YAML::Key << "Color" << YAML::Value << light->GetColor();
            out << YAML::EndMap; // End LightComponent
        }
        if (entity->HasComponent<CMeshComponent>())
        {
            const SharedPtr<IMesh> mesh = entity->GetComponent<CMeshComponent>()->GetMesh();
            const SharedPtr<IMaterial> material = mesh->GetMaterial();
            if (mesh && material)
            {
                out << YAML::Key << "MeshComponent";
                out << YAML::BeginMap; // Begin MeshComponent
                out << YAML::Key << "Mesh" << YAML::Value << mesh->GetPath().c_str();
                out << YAML::Key << "Material";
                out << YAML::BeginMap; // Begin Material
                out << YAML::Key << "Ambient" << YAML::Value << material->GetAmbient();
                out << YAML::Key << "Diffuse" << YAML::Value << material->GetDiffuse();
                out << YAML::Key << "Specular" << YAML::Value << material->GetSpecular();
                out << YAML::Key << "Shininess" << YAML::Value << material->GetShininess();
                out << YAML::Key << "IsUsingTexture" << YAML::Value << material->IsUsingTexture();
                out << YAML::EndMap; // End Material
                out << YAML::EndMap; // End MeshComponent 
            }
        }
        if (entity->HasComponent<CSpriteRenderComponent>())
        {
            CSpriteRenderComponent* sprite = entity->GetComponent<CSpriteRenderComponent>();
            out << YAML::Key << "SpriteRenderComponent";
            out << YAML::BeginMap; // Begin SpriteRenderComponent
            out << YAML::Key << "Color" << YAML::Value << sprite->GetColor();
            out << YAML::Key << "Texture";
            const SharedPtr<ITexture> texture = sprite->GetTexture();
            if (texture)
                out << YAML::Value << texture->GetPath();
            else
                out << YAML::Value << "Empty";
            out << YAML::EndMap; // End SpriteRenderComponent
        }
        if (entity->HasComponent<CNativeScriptComponent>())
        {
            CNativeScriptComponent* script = entity->GetComponent<CNativeScriptComponent>();
            // Temp
            const CType& scriptType = script->GetDynamicType();
            out << YAML::Key << "NativeScriptComponent";
            out << YAML::BeginMap; // Begin NativeScriptComponent
            out << YAML::Key << "ScriptType" << YAML::Value << scriptType.GetName().c_str();
            out << YAML::EndMap; // End NativeScriptComponent
        }

        out << YAML::EndMap; // End entity
    }

    static void DeserializeEntity(CEntity* entity, const YAML::Node& entityNode)
    {
        PROFILE_SCOPE(EntityDeserialization);
        const uint32_t handle = entityNode["EntityHandle"].as<uint32_t>();
        const std::string name = entityNode["TagComponent"]["Tag"].as<std::string>();
        VOLT_LOG(Info, "Deserializating {0}:{1}...", handle, name.c_str());
        CTagComponent* tag = entity->GetComponent<CTagComponent>();
        tag->SetTag(name);

        const YAML::Node transformNode = entityNode["TransformComponent"];
        CTransformComponent* transform = entity->GetComponent<CTransformComponent>();
        transform->SetPosition(transformNode["Translation"].as<glm::vec3>());
        transform->SetRotation(transformNode["Rotation"].as<glm::vec3>());
        transform->SetScale(transformNode["Scale"].as<glm::vec3>());

        if (const YAML::Node cameraNode = entityNode["CameraComponent"])
        {
            CCameraComponent* camera = entity->AddComponent<CCameraComponent>();
            camera->SetPrimary(cameraNode["Primary"].as<bool>());
        }
        if (const YAML::Node lightNode = entityNode["LightComponent"])
        {
            CLightComponent* light = entity->AddComponent<CLightComponent>();
            light->SetType(static_cast<ELightType>(lightNode["Type"].as<uint32_t>()));
            light->SetColor(lightNode["Color"].as<glm::vec4>());
        }
        if (const YAML::Node meshNode = entityNode["MeshComponent"])
        {
            const YAML::Node materialNode = meshNode["Material"];

            CMeshComponent* meshComponent = entity->AddComponent<CMeshComponent>();
            SharedPtr<IMesh> mesh = CAssetManager::LoadAsset<IMesh>(meshNode["Mesh"].as<std::string>());
            SharedPtr<IMaterial> material = mesh->GetMaterial();
            VOLT_ASSERT(mesh && material, "Failed on load mesh while loading entity {0}", name.c_str());
            material->SetAmbient(materialNode["Ambient"].as<glm::vec4>());
            material->SetDiffuse(materialNode["Diffuse"].as<glm::vec4>());
            material->SetSpecular(materialNode["Specular"].as<glm::vec4>());
            material->SetShininess(materialNode["Shininess"].as<float>());
            material->UseTexture(materialNode["IsUsingTexture"].as<bool>());
            meshComponent->SetMesh(mesh);
        }
        if (const YAML::Node spriteNode = entityNode["SpriteRenderComponent"])
        {
            CSpriteRenderComponent* sprite = entity->AddComponent<CSpriteRenderComponent>();
            sprite->SetColor(spriteNode["Color"].as<glm::vec4>());
            const std::string texturePath = spriteNode["Texture"].as<std::string>();
            if (texturePath != "Empty")
            {
                sprite->SetTexture(CAssetManager::LoadAsset<ITexture>(texturePath));
            }
        }
        if (const YAML::Node scriptNode = entityNode["NativeScriptComponent"])
        {
            // TODO
        }
    }


    CSceneSerializer::CSceneSerializer(CScene* sceneContext)
        : m_scene(sceneContext)
    {
        VOLT_ASSERT(m_scene, "Invalid scene context.");
    }

    void CSceneSerializer::Serialize(const std::string& filepath)
    {
        PROFILE_SCOPE(Serialization);
        const std::filesystem::path path = std::filesystem::absolute(filepath).lexically_normal();
        VOLT_LOG(Info, "Serializing scene to file: {0}", path.string().c_str());

        YAML::Emitter out;
        out << YAML::BeginMap; // Begin scene
        out << YAML::Key << "Scene" << YAML::Value << "Untitled";
        out << YAML::Key << "Render";
        out << YAML::BeginMap; // Begin Render
        out << YAML::Key << "Ambient color" << YAML::Value << CRenderer3D::AmbientColor;
        out << YAML::Key << "Ambient strength" << YAML::Value << CRenderer3D::AmbientStrength;
        out << YAML::EndMap; // End Render
        out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq; // Begin Entities
        m_scene->GetRegistry().Each([&](CEntity* entity)
        {
            if (entity)
            {
                SerializeEntity(out, entity);
            }
        });
        out << YAML::EndSeq; // End Entities
        out << YAML::EndMap; // End scene

        std::ofstream fileout(path.c_str());
        VOLT_ASSERT(fileout.is_open(), "Couldn't write on file {0}", path.string().c_str());
        fileout << out.c_str();
        fileout.close();
    }

    bool CSceneSerializer::Deserialize(const std::string& filepath)
    {
        PROFILE_SCOPE(Deserialization);
        const std::filesystem::path path = std::filesystem::absolute(filepath);
        VOLT_LOG(Info, "Deserializing scene from file {0}", path.string().c_str());

        const std::ifstream stream(path.string().c_str());
        std::stringstream buffer;
        buffer << stream.rdbuf();

        const YAML::Node root = YAML::Load(buffer.str());
        const std::string sceneName = root["Scene"].as<std::string>();
        VOLT_LOG(Info, "Loading {0} scene...", sceneName.c_str());

        const YAML::Node render = root["Render"];
        CRenderer3D::AmbientColor = render["Ambient color"].as<glm::vec3>();
        CRenderer3D::AmbientStrength = render["Ambient strength"].as<float>();

        const YAML::Node entitiesRoot = root["Entities"];
        if (!entitiesRoot.IsNull())
        {
            for (YAML::detail::iterator_value entityNode : entitiesRoot)
            {
                CEntity* entity = m_scene->CreateEntity();
                DeserializeEntity(entity, entityNode);
            }
        }
        else
            VOLT_LOG(Error, "Invalid yaml format {0}", filepath.c_str());

        return true;
    }
}
