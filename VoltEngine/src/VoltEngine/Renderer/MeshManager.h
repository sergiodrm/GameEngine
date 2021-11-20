#pragma once
#include "Mesh.h"

namespace Volt
{
    class CMeshManager : public IResourceManager<IMesh>, public ISingleton<CMeshManager>
    {
    public:
        virtual void OnCreate() override;
        virtual void OnDestroy() override;
    protected:
        virtual ResourcePtr CreateResource() override;
    };
}
