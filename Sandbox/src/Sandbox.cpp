#include "Sandbox.h"


CSandboxApplication::CSandboxApplication()
{
    VOLT_LOG(Info, "Creating sandbox application");
}

CSandboxApplication::~CSandboxApplication()
{
    VOLT_LOG(Info, "Destroying sandbox application");
}

Volt::CApplication* CreateApplication()
{
    return new CSandboxApplication();
}
