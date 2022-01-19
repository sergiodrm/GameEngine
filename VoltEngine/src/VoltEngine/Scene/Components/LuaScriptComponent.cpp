#include "LuaScriptComponent.h"

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "VoltEngine/Core/Log.h"

class CLuaState
{
public:

    static void RegisterScript()
    {
        if (s_instance == nullptr)
        {
            s_instance = new CLuaState();
        }
        ++s_countRefs;
    }

    static lua_State* GetState()
    {
        VOLT_ASSERT(s_instance, "There is no script subscribed to CLuaState.");
        return s_instance->m_luaState;
    }

    static void FreeScript()
    {
        VOLT_ASSERT(s_instance, "CLuaState::Instance is nullptr");
        --s_countRefs;
        if (s_countRefs == 0)
        {
            delete s_instance;
            s_instance = nullptr;
        }
    }

private:
    CLuaState()
    {
        m_luaState = luaL_newstate();
        luaL_openlibs(m_luaState);
    }

    ~CLuaState()
    {
        lua_close(m_luaState);
    }

private:
    static CLuaState* s_instance;
    static uint32_t s_countRefs;
    lua_State* m_luaState{nullptr};
};

CLuaState* CLuaState::s_instance = nullptr;
uint32_t CLuaState::s_countRefs = 0;

namespace Volt
{
    CLuaScriptComponent::CLuaScriptComponent()
    {
        CLuaState::RegisterScript();
    }

    CLuaScriptComponent::~CLuaScriptComponent()
    {
        CLuaState::FreeScript();
    }

    void CLuaScriptComponent::LoadScript(const std::string& scriptPath)
    {
        
    }

    void CLuaScriptComponent::RunScript()
    {
        
    }

}