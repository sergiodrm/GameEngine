#include "Sandbox.h"

#include <cstdio>

CSandboxApplication::CSandboxApplication()
{
    printf("Creating Sandbox application\n");
}

CSandboxApplication::~CSandboxApplication()
{
    printf("Destroying Sandbox application\n");
}

Volt::CApplication* CreateApplication()
{
    return new CSandboxApplication();
}
