#pragma once
#include "VoltEngine/Scene/Component.h"

namespace Volt
{
    class CLuaScriptComponent : public CComponent
    {
        DECLARE_DERIVED_CLASS(CLuaScriptComponent, CComponent)

    public:

        CLuaScriptComponent();
        virtual ~CLuaScriptComponent() override;

        // Temp
        void LoadScript(const std::string& scriptPath);
        void RunScript();
        
    };
}
