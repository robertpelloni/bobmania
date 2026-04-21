#include "PluginManager.h"
#include "RageLog.h"

// Includes for Engine API bindings
#include "Economy/EconomyManager.h"
#include "Gym/HeartRateManager.h"
// #include "RageInput.h"

PluginManager* PLUGINMAN = nullptr;

PluginManager::PluginManager() {}

PluginManager::~PluginManager() {}

void PluginManager::Init() {
    LOG->Info("PluginManager::Init() - Scanning Plugins/ directory...");

    // Stub: In reality, we'd use RageFile to iter over the Plugins/ folder
    // looking for .dll (Windows) or .so (Linux) files.

    // For MVP, we pretend we found one.
    // void* handle = LoadDynamicLibrary("Plugins/CustomDancePad.dll");
    // if(handle) {
    //     void (*initFunc)() = (void(*)())GetFunctionPointer(handle, "InitializePlugin");
    //     if(initFunc) initFunc();
    //
    //     LoadedPlugin p;
    //     p.sName = "CustomDancePad";
    //     p.pHandle = handle;
    //     p.pUpdateFunc = (void(*)(float))GetFunctionPointer(handle, "UpdatePlugin");
    //     m_vPlugins.push_back(p);
    // }
}

void PluginManager::Shutdown() {
    LOG->Info("PluginManager::Shutdown() - Unloading plugins.");
    for (size_t i = 0; i < m_vPlugins.size(); ++i) {
        void (*shutdownFunc)() = (void(*)())GetFunctionPointer(m_vPlugins[i].pHandle, "ShutdownPlugin");
        if(shutdownFunc) shutdownFunc();

        UnloadDynamicLibrary(m_vPlugins[i].pHandle);
    }
    m_vPlugins.clear();
}

void PluginManager::Update(float fDeltaTime) {
    for (size_t i = 0; i < m_vPlugins.size(); ++i) {
        if (m_vPlugins[i].pUpdateFunc) {
            m_vPlugins[i].pUpdateFunc(fDeltaTime);
        }
    }
}

// ---------------------------------------------------------
// OS-Specific Loading Stubs (Abstracted for Compilation Check)
// ---------------------------------------------------------
void* PluginManager::LoadDynamicLibrary(const RString& path) {
    // Windows: LoadLibrary(path.c_str());
    // Unix: dlopen(path.c_str(), RTLD_NOW);
    return nullptr;
}

void* PluginManager::GetFunctionPointer(void* handle, const RString& funcName) {
    // Windows: GetProcAddress((HMODULE)handle, funcName.c_str());
    // Unix: dlsym(handle, funcName.c_str());
    return nullptr;
}

void PluginManager::UnloadDynamicLibrary(void* handle) {
    // Windows: FreeLibrary((HMODULE)handle);
    // Unix: dlclose(handle);
}

// ---------------------------------------------------------
// C Plugin API Implementation (Engine Side)
// ---------------------------------------------------------

extern "C" {
    long long GetBobcoinBalance() {
        if(ECONOMYMAN) return ECONOMYMAN->GetBalance();
        return 0;
    }

    void InjectHardwareInput(int playerNum, int column, bool isPressed) {
        // Stub for routing to RageInput
        // INPUTFILTER->ButtonPressed(...);
    }

    int GetLiveHeartRate() {
        if(HEARTRATEMAN) return HEARTRATEMAN->GetBPM();
        return 0;
    }

    void LogMessage(const char* message) {
        LOG->Info("[PLUGIN] %s", message);
    }
}
