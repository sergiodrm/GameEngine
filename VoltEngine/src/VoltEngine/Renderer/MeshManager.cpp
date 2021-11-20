#include "MeshManager.h"

namespace Volt
{
    void CMeshManager::OnCreate()
    {
        VOLT_LOG(Trace, "Creating Mesh manager...");
    }

    void CMeshManager::OnDestroy()
    {
        VOLT_LOG(Trace, "Destroying Mesh manager...");
    }

    IResourceManager<IMesh>::ResourcePtr CMeshManager::CreateResource()
    {
        return IMesh::Create();
    }
}
