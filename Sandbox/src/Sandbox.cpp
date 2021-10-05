#include "Sandbox.h"

#include "GameLayer.h"


CSandboxApplication::CSandboxApplication()
{
    VOLT_LOG(Info, "Creating sandbox application");
    PushLayer(new CGameLayer());
}

CSandboxApplication::~CSandboxApplication()
{
    VOLT_LOG(Info, "Destroying sandbox application");
}

Volt::CApplication* CreateApplication()
{
    return new CSandboxApplication();
}
