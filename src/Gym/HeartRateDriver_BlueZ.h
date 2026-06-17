#ifndef HEARTRATE_DRIVER_BLUEZ_H
#define HEARTRATE_DRIVER_BLUEZ_H

#include "IHeartRateDriver.h"

/**
 * Linux-specific Heart Rate Driver using BlueZ (DBus) APIs.
 */
class HeartRateDriver_BlueZ : public IHeartRateDriver
{
public:
    HeartRateDriver_BlueZ();
    virtual ~HeartRateDriver_BlueZ();

    virtual bool Init() override;
    virtual void Shutdown() override;
    virtual int GetHeartRate() override;
    virtual bool IsConnected() override;

private:
    bool m_bConnected;
};

#endif
