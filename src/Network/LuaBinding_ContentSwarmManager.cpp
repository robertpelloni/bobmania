#include "LuaBinding_ContentSwarmManager.h"

int LunaContentSwarmManager::StartDownload(T* p, lua_State* L) {
    RString hash = SArg(1);
    bool result = p->StartDownload(hash);
    lua_pushboolean(L, result ? 1 : 0);
    return 1;
}

int LunaContentSwarmManager::CancelDownload(T* p, lua_State* L) {
    RString hash = SArg(1);
    p->CancelDownload(hash);
    return 0;
}

int LunaContentSwarmManager::GetDownloadProgress(T* p, lua_State* L) {
    RString hash = SArg(1);
    lua_pushnumber(L, p->GetDownloadProgress(hash));
    return 1;
}

int LunaContentSwarmManager::GetSeedCount(T* p, lua_State* L) {
    RString hash = SArg(1);
    lua_pushnumber(L, p->GetSeedCount(hash));
    return 1;
}

int LunaContentSwarmManager::GetLeechCount(T* p, lua_State* L) {
    RString hash = SArg(1);
    lua_pushnumber(L, p->GetLeechCount(hash));
    return 1;
}

int LunaContentSwarmManager::IsDownloadComplete(T* p, lua_State* L) {
    RString hash = SArg(1);
    lua_pushboolean(L, p->IsDownloadComplete(hash) ? 1 : 0);
    return 1;
}

LUA_REGISTER_CLASS(ContentSwarmManager)

/**
 * Register SWARMMAN as a global singleton in Lua.
 */
class LunaContentSwarmManagerGlobal
{
public:
    static void Register(lua_State* L)
    {
        lua_pushstring(L, "SWARMMAN");
        if (SWARMMAN == nullptr) {
            lua_pushnil(L);
        } else {
            LunaContentSwarmManager::Push(L, SWARMMAN);
        }
        lua_settable(L, LUA_GLOBALSINDEX);
    }
};

REGISTER_WITH_LUA_FUNCTION(LunaContentSwarmManagerGlobal::Register);
