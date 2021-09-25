#include <iostream>

#include "Application.h"

extern Volt::CApplication* CreateApplication();

int main()
{
    Volt::CApplication* app = CreateApplication();
    app->Run();
    delete app;
    return 0;
}
