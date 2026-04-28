#ifndef PLUGIN_MANAGER_H
#define PLUGIN_MANAGER_H

#include "global.h"
#include <vector>

class PluginManager
{
public:
    PluginManager();
    ~PluginManager();

    void Init();
    void Shutdown();

    void Update(float fDeltaTime);

private:
    struct LoadedPlugin {
        RString sName;
        void* pHandle; // OS-specific handle (HMODULE or void*)
        void (*pUpdateFunc)(float); // Function pointer to the plugin's Update()
    };

    std::vector<LoadedPlugin> m_vPlugins;

    // Platform-specific library loading stubs
    void* LoadDynamicLibrary(const RString& path);
    void* GetFunctionPointer(void* handle, const RString& funcName);
    void UnloadDynamicLibrary(void* handle);
};

extern PluginManager* PLUGINMAN;

#endif
