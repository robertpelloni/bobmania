#ifndef HEARTRATE_MANAGER_H
#define HEARTRATE_MANAGER_H

#include "global.h"
#include "IHeartRateDriver.h"

/**
 * Global singleton that manages the heart rate hardware.
 * Accessible via HEARTRATEMAN.
 */
class HeartRateManager
{
public:
    HeartRateManager();
    ~HeartRateManager();

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

#endif
