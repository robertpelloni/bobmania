<<<<<<< HEAD
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
=======
#include "global.h"
#include "HeartRateManager.h"
#include "RageLog.h"
#include "LuaBinding.h"
#include "RageMath.h"

HeartRateManager* HRMAN = nullptr;

HeartRateManager::HeartRateManager()
{
    m_iCurrentBPM = 0;
    m_fTimer = 0.0f;
    m_bConnected = true; // Simulating a connection
}

HeartRateManager::~HeartRateManager()
{
}

void HeartRateManager::Init()
{
    LOG->Trace("HeartRateManager::Init()");
}

void HeartRateManager::Update( float fDeltaTime )
{
    m_fTimer += fDeltaTime;

    // Simulate Heart Rate: Sine wave between 80 and 140 BPM
    // Period = 10 seconds
    // RageFastSin requires CONST_FUNCTION or standard math
    float fSine = sin( m_fTimer * 0.6f );
    m_iCurrentBPM = 110 + (int)(fSine * 30);
}

int HeartRateManager::GetHeartRate() const
{
    return m_iCurrentBPM;
}

bool HeartRateManager::IsConnected() const
{
    return m_bConnected;
}

// Lua Bindings
class LunaHeartRateManager: public Luna<HeartRateManager>
{
public:
    static int GetHeartRate( T* p, lua_State *L )
    {
        lua_pushnumber(L, p->GetHeartRate());
        return 1;
    }

    static int IsConnected( T* p, lua_State *L )
    {
        lua_pushboolean(L, p->IsConnected());
        return 1;
    }

    LunaHeartRateManager()
    {
        ADD_METHOD( GetHeartRate );
        ADD_METHOD( IsConnected );
    }
};

LUA_REGISTER_CLASS( HeartRateManager )

/*
 * ==============================================================================
 * UNIFIED STEPMANIA ARCHITECTURE NOTES: HeartRateManager
 * ==============================================================================
 * This class currently outputs a sine wave in `Update()` to simulate a live
 * BPM reading for testing `ScreenGymWorkout`.
 *
 * NEXT IMPLEMENTOR:
 * 1. Remove the sine wave logic.
 * 2. Implement an abstract `IHeartRateDriver`.
 * 3. Write a `HeartRateDriver_BluetoothLE` subclass (using OS-specific APIs
 *    like CoreBluetooth on Mac, WinRT on Windows).
 * 4. The manager should poll the driver, handle disconnects gracefully, and
 *    fallback to 0 BPM without crashing the game loop.
 * ==============================================================================
 */
>>>>>>> origin/unified-ui-features-13937230807013224518
