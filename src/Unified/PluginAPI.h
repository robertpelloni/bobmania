#ifndef PLUGIN_API_H
#define PLUGIN_API_H

/**
 * Unified StepMania - C Plugin API
 *
 * This header defines a stable C-ABI that allows modders to compile external
 * .dll or .so files (e.g. for custom dance pad lighting or hardware integrations)
 * without needing the massive 2-million-line C++ codebase.
 */

#ifdef _WIN32
  #define PLUGIN_EXPORT __declspec(dllexport)
#else
  #define PLUGIN_EXPORT __attribute__((visibility("default")))
#endif

extern "C" {

    // ---------------------------------------------------------
    // Core Engine Callbacks (Implemented by the Plugin)
    // ---------------------------------------------------------

    // Called once when the plugin is loaded by PluginManager
    PLUGIN_EXPORT void InitializePlugin();

    // Called once when the engine shuts down
    PLUGIN_EXPORT void ShutdownPlugin();

    // Called every frame. Use for polling custom hardware.
    PLUGIN_EXPORT void UpdatePlugin(float fDeltaTime);

    // ---------------------------------------------------------
    // Engine API (Callable by the Plugin)
    // ---------------------------------------------------------

    // Retrieve the player's Bobcoin balance from ECONOMYMAN
    long long GetBobcoinBalance();

    // Issue a simulated hardware input to the engine (e.g. for custom panels)
    void InjectHardwareInput(int playerNum, int column, bool isPressed);

    // Get the current player's live heart rate from GYMMAN
    int GetLiveHeartRate();

    // Log a message to the engine's standard log file
    void LogMessage(const char* message);
}

#endif
