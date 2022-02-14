#include "QtEditor.h"
#include "VoltEngine/VoltEngine.h"


class CQtEditorApplication : public Volt::CApplication
{
public:
    CQtEditorApplication()
    {
        VOLT_LOG(Info, "Init QtEditorApplication");
    }

    virtual ~CQtEditorApplication() override
    {
        VOLT_LOG(Info, "Shutdown QtEditorApplication");
    }
};

Volt::CApplication* CreateApplication()
{
    return new CQtEditorApplication();
}
