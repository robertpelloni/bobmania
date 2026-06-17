#include "HeartRateDriver_BlueZ.h"
#include "RageLog.h"

HeartRateDriver_BlueZ::HeartRateDriver_BlueZ() : m_bConnected(false) {}

HeartRateDriver_BlueZ::~HeartRateDriver_BlueZ() {
    Shutdown();
}

bool HeartRateDriver_BlueZ::Init() {
    LOG->Trace("HeartRateDriver_BlueZ: Connecting to BlueZ via DBus...");
    return true;
}

void HeartRateDriver_BlueZ::Shutdown() {
    m_bConnected = false;
}

int HeartRateDriver_BlueZ::GetHeartRate() {
    return 0;
}

bool HeartRateDriver_BlueZ::IsConnected() {
    return m_bConnected;
}
