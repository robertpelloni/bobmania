#ifndef HEARTRATE_DRIVER_MOCK_H
#define HEARTRATE_DRIVER_MOCK_H

#include "IHeartRateDriver.h"

class HeartRateDriver_Mock : public IHeartRateDriver
{
public:
    HeartRateDriver_Mock();
    ~HeartRateDriver_Mock() override;

    bool Init() override;
    void Shutdown() override;
    int GetHeartRate() override;
    bool IsConnected() override;
private:
    float m_fTimeElapsed;
};

#endif
