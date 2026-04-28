#include "HeartRateDriver_Mock.h"
#include <math.h>

HeartRateDriver_Mock::HeartRateDriver_Mock() : m_fTimeElapsed(0.0f) {}

HeartRateDriver_Mock::~HeartRateDriver_Mock() {}

bool HeartRateDriver_Mock::Init() {
    return true; // Always succeeds
}

void HeartRateDriver_Mock::Shutdown() {
    // Nothing to clean up
}

int HeartRateDriver_Mock::GetHeartRate() {
    // Return a sine wave simulating 80-140 BPM
    // Note: In a real implementation we'd use RageTimer, but this is a simple mock
    m_fTimeElapsed += 0.016f; // Rough approximation of 60fps
    return 110 + (int)(sin(m_fTimeElapsed * 0.5f) * 30.0f);
}

bool HeartRateDriver_Mock::IsConnected() {
    return true;
}
