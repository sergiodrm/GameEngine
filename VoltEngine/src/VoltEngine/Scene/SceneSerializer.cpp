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
#include "VoltEngine/Core/Log.h"
#include "VoltEngine/Renderer/Mesh.h"
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
            SharedPtr<IMesh> mesh = entity->GetComponent<CMeshComponent>()->GetMesh();
            if (mesh)
            {
                out << YAML::Key << "MeshComponent";
                out << YAML::BeginMap; // Begin MeshComponent
                out << YAML::Key << "Mesh" << YAML::Value << mesh->GetPath().c_str();
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

        out << YAML::EndMap; // Begin entity
    }


    CSceneSerializer::CSceneSerializer(CScene* sceneContext)
        : m_scene(sceneContext)
    {
        VOLT_ASSERT(m_scene, "Invalid scene context.");
    }

    void CSceneSerializer::Serialize(const std::string& filepath)
    {
        const std::filesystem::path path = std::filesystem::absolute(filepath).lexically_normal();
        VOLT_LOG(Info, "Serializing scene to file: {0}", path.c_str());

        YAML::Emitter out;
        out << YAML::BeginMap; // Begin scene
        out << YAML::Key << "Scene" << YAML::Value << "Untitled";
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
        VOLT_ASSERT(fileout.is_open(), "Couldn't write on file {0}", path.c_str());
        fileout << out.c_str();
        fileout.close();
    }

    void CSceneSerializer::Deserialize(const std::string& filepath)
    { }
}
