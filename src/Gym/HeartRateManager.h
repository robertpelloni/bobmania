<<<<<<< HEAD
#ifndef HEARTRATE_MANAGER_H
#define HEARTRATE_MANAGER_H

#include "global.h"
#include "IHeartRateDriver.h"

/**
 * Global singleton that manages the heart rate hardware.
 * Accessible via HEARTRATEMAN.
 */
=======
#ifndef HEART_RATE_MANAGER_H
#define HEART_RATE_MANAGER_H

#include "RageUtil.h"

struct lua_State;

>>>>>>> origin/unified-ui-features-13937230807013224518
class HeartRateManager
{
public:
    HeartRateManager();
    ~HeartRateManager();

<<<<<<< HEAD
    // Initialize the best available driver for the platform
    void Init();

    // Shut down the driver
    void Shutdown();

    // Update logic, called once per frame in GameLoop
    void Update(float fDeltaTime);

    // Returns the current Beats Per Minute
    int GetBPM() const;

    // Returns true if hardware is actively transmitting
    bool IsConnected() const;

private:
    IHeartRateDriver* m_pDriver;
    int m_iCurrentBPM;
};

extern HeartRateManager* HEARTRATEMAN;
=======
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
>>>>>>> origin/unified-ui-features-13937230807013224518

#endif
