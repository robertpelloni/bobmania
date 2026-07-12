#include "HeartRateManager.h"
#include "HeartRateDriver_Mock.h"

#ifdef _WIN32
#include "HeartRateDriver_WinRT.h"
#elif defined(LINUX)
#include "HeartRateDriver_BlueZ.h"
#endif

HeartRateManager* HEARTRATEMAN = nullptr;

HeartRateManager::HeartRateManager() : m_pDriver(nullptr), m_iCurrentBPM(0) {}

HeartRateManager::~HeartRateManager() {
}

void HeartRateManager::Init() {
    // Determine which driver to use based on OS.
#ifdef _WIN32
    m_pDriver = std::make_unique<HeartRateDriver_WinRT>();
#elif defined(LINUX)
    m_pDriver = std::make_unique<HeartRateDriver_BlueZ>();
#else
    m_pDriver = std::make_unique<HeartRateDriver_Mock>();
#endif

    if (m_pDriver && !m_pDriver->Init()) {
        m_pDriver.reset();
    }
}

void HeartRateManager::Shutdown() {
    if (m_pDriver) {
        m_pDriver->Shutdown();
        m_pDriver.reset();
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
