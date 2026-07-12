#ifndef HEARTRATE_DRIVER_WINRT_H
#define HEARTRATE_DRIVER_WINRT_H

#include "IHeartRateDriver.h"

/**
 * Windows-specific Heart Rate Driver using WinRT Bluetooth LE APIs.
 * This is currently a stub preparing for C++/WinRT integration.
 */
class HeartRateDriver_WinRT : public IHeartRateDriver
{
public:
    HeartRateDriver_WinRT();
    virtual ~HeartRateDriver_WinRT();

    virtual bool Init() override;
    virtual void Shutdown() override;
    virtual int GetHeartRate() override;
    virtual bool IsConnected() override;

private:
    bool m_bConnected;
};

#endif
