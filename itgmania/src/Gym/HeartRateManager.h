#ifndef HEART_RATE_MANAGER_H
#define HEART_RATE_MANAGER_H

#include "RageUtil.h"

struct lua_State;

class HeartRateManager
{
public:
    HeartRateManager();
    ~HeartRateManager();

    void Init();
    void Update( float fDeltaTime );

    int GetHeartRate() const;
    bool IsConnected() const;

    // Lua
    void PushSelf( lua_State *L );

private:
    int m_iCurrentBPM;
    float m_fTimer;
    bool m_bConnected;
};

extern HeartRateManager* HRMAN;

#endif
