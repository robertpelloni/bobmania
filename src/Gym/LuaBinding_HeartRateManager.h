#ifndef LUABINDING_HEARTRATE_MANAGER_H
#define LUABINDING_HEARTRATE_MANAGER_H

#include "HeartRateManager.h"
#include "LuaBinding.h"

class LunaHeartRateManager : public Luna<HeartRateManager>
{
public:
    static int GetBPM(T* p, lua_State* L);
    static int IsConnected(T* p, lua_State* L);

    LunaHeartRateManager()
    {
        ADD_METHOD(GetBPM);
        ADD_METHOD(IsConnected);
    }
};

#endif
