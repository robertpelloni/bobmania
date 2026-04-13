#ifndef I_HEARTRATE_DRIVER_H
#define I_HEARTRATE_DRIVER_H

#include "global.h"

/**
 * Interface for hardware heart rate drivers (e.g., Bluetooth LE monitors).
 * Implementations should poll the hardware asynchronously and return the latest BPM.
 */
class IHeartRateDriver
{
public:
    virtual ~IHeartRateDriver() {}

    // Initialize connection to hardware
    virtual bool Init() = 0;

    // Shut down connection
    virtual void Shutdown() = 0;

    // Return current BPM
    virtual int GetHeartRate() = 0;

    // Returns true if device is currently connected and transmitting
    virtual bool IsConnected() = 0;
};

#endif
