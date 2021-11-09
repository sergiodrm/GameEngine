#pragma once
#include "VoltEngine/Core/Core.h"


namespace Volt
{
    class ITexture;

    class CTextureLoader
    {
    public:
        static Ref<ITexture> Load(const std::string& filePath);
        static bool IsLoaded(const std::string& filePath);
        static Ref<ITexture> Find(const std::string& filePath);
        static void Unload(const std::string& filePath);

    private:
        static std::unordered_map<std::string, Ref<ITexture>> m_loadedTextures;
    };
}
