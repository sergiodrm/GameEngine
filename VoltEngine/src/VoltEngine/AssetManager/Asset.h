#pragma once

#include <filesystem>

#include "VoltEngine/Core/Log.h"
#include "VoltEngine/RTTI/TypeMacros.h"

namespace Volt
{
    class IAsset
    {
    DECLARE_BASE_CLASS(IAsset);
        friend class CAssetManager;
    public:
        virtual ~IAsset() = default;
        virtual UniquePtr<class IAssetLoader> CreateLoader() = 0;
        virtual bool IsLoaded() const = 0;

        virtual std::string GetName() const { return m_path.filename().string(); }
        virtual std::string GetPath() const { return m_path.string(); }

    private:
        virtual void SetPath(const std::string& path)
        {
            const std::filesystem::path tempPath(path);
            VOLT_ASSERT(!tempPath.empty() && tempPath.has_extension() && tempPath.has_filename() && tempPath.is_absolute(), "Invalid path format");
            m_path = tempPath;
        }

    private:
        std::filesystem::path m_path;
    };
}
