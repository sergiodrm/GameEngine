#pragma once
#include <string>


namespace Volt
{
    class CEntity;
    class CScene;

    class CSceneSerializer
    {
    public:
        CSceneSerializer(CScene* sceneContext);

        void Serialize(const std::string& filepath);
        bool Deserialize(const std::string& filepath);

    private:
        CScene* m_scene {nullptr};
    };
}
