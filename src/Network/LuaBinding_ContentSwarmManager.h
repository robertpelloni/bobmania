#ifndef LUABINDING_CONTENT_SWARM_MANAGER_H
#define LUABINDING_CONTENT_SWARM_MANAGER_H

#include "ContentSwarmManager.h"
#include "LuaBinding.h"

class LunaContentSwarmManager : public Luna<ContentSwarmManager>
{
public:
    static int StartDownload(T* p, lua_State* L);
    static int CancelDownload(T* p, lua_State* L);
    static int GetDownloadProgress(T* p, lua_State* L);
    static int GetSeedCount(T* p, lua_State* L);
    static int GetLeechCount(T* p, lua_State* L);
    static int IsDownloadComplete(T* p, lua_State* L);

    LunaContentSwarmManager()
    {
        ADD_METHOD(StartDownload);
        ADD_METHOD(CancelDownload);
        ADD_METHOD(GetDownloadProgress);
        ADD_METHOD(GetSeedCount);
        ADD_METHOD(GetLeechCount);
        ADD_METHOD(IsDownloadComplete);
    }
};

#endif
