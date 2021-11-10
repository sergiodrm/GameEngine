#include "VoltEditor.h"

#include "EditorLayer.h"

CVoltEditor::CVoltEditor()
{
    VOLT_LOG(Info, "Creating Volt Editor instance!");
    PushLayer(new CEditorLayer());
}

CVoltEditor::~CVoltEditor()
{
    VOLT_LOG(Info, "Destroying Volt Editor instance!");
}


Volt::CApplication* CreateApplication()
{
    return new CVoltEditor();
}
