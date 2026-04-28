#include "LuaBinding_HeartRateManager.h"
#include "HeartRateManager.h"

int LunaHeartRateManager::GetBPM(T* p, lua_State* L) {
    lua_pushnumber(L, p->GetBPM());
    return 1;
}

int LunaHeartRateManager::IsConnected(T* p, lua_State* L) {
    lua_pushboolean(L, p->IsConnected() ? 1 : 0);
    return 1;
}

LUA_REGISTER_CLASS(HeartRateManager)

/**
 * Register HEARTRATEMAN as a global singleton in Lua.
 */
class LunaHeartRateManagerGlobal
{
public:
    static void Register(lua_State* L)
    {
        lua_pushstring(L, "HEARTRATEMAN");
        if (HEARTRATEMAN == nullptr) {
            lua_pushnil(L);
        } else {
            LunaHeartRateManager::Push(L, HEARTRATEMAN);
        }
        lua_settable(L, LUA_GLOBALSINDEX);
    }
};

REGISTER_WITH_LUA_FUNCTION(LunaHeartRateManagerGlobal::Register);
