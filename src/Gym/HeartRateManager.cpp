#include "HeartRateManager.h"
#include "HeartRateDriver_Mock.h"
// In the future: #include "HeartRateDriver_BlueZ.h" (Linux), "HeartRateDriver_WinRT.h" (Windows)

HeartRateManager* HEARTRATEMAN = nullptr;

HeartRateManager::HeartRateManager() : m_pDriver(nullptr), m_iCurrentBPM(0) {}

HeartRateManager::~HeartRateManager() {
    Shutdown();
}

void HeartRateManager::Init() {
    // Determine which driver to use. For now, use the Mock.
    // In the future, this would be determined by #ifdef WIN32 / LINUX
    m_pDriver = new HeartRateDriver_Mock();

    if (m_pDriver && !m_pDriver->Init()) {
        delete m_pDriver;
        m_pDriver = nullptr;
    }
}

void HeartRateManager::Shutdown() {
    if (m_pDriver) {
        m_pDriver->Shutdown();
        delete m_pDriver;
        m_pDriver = nullptr;
    }
}

void HeartRateManager::Update(float fDeltaTime) {
    if (m_pDriver && m_pDriver->IsConnected()) {
        m_iCurrentBPM = m_pDriver->GetHeartRate();
    } else {
        m_iCurrentBPM = 0; // 0 indicates no reading
    }
}

int HeartRateManager::GetBPM() const {
    return m_iCurrentBPM;
}

bool HeartRateManager::IsConnected() const {
    return m_pDriver != nullptr && m_pDriver->IsConnected();
}
