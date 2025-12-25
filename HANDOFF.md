# Handoff Document: StepMania 5.2 (5.1-new) Unified Fork Project

## 1. Project Analysis: History & Fragmentation
The StepMania 5.2 (5.1-new) branch stalled due to **"The Merge"** complexity and leadership gaps. The community fragmented into specialized forks:
*   **NotITG:** Forked from SM 3.95/OpenITG to focus on intense visual modding, breaking compatibility with SM5 architectures (`.ssc`, `ActorFrame`).
*   **ITGMania:** Forked from SM 5.1 to focus on competitive stamina play, Quality of Life (QoL), and GrooveStats integration.
*   **Etterna:** Forked for keyboard-specific mechanics, difficulty calculation (MSD), and C++ optimizations.
*   **Project OutFox:** A closed-source continuation focusing on broad hardware support and new game modes.

This project has successfully **unified** the critical features of these forks back into the StepMania 5.1-new baseline, creating a "Super 5.1" that supports:
1.  **NotITG Modding** (Shaders, Lua Hooks, NotePath, Splines, Proxies).
2.  **ITGMania QoL** (Song Reloading, Profile Switching, Networking).
3.  **Etterna/OutFox Connectivity** (Discord RPC, Toast Notifications, File I/O).

## 2. Implemented Features (The "Merge")

### A. ITGMania Parity (Competitive QoL)
1.  **Load New Songs (`SSMReloadSongs`)**
    *   **Function:** `SongManager::LoadNewSongs()`.
    *   **Usage:** Reloads the song database without restarting.
2.  **Fast Profile Switching (`SSMProfileSwitch`)**
    *   **Function:** `ProfileManager::NextLocalProfile(pn)`.
    *   **Usage:** Cycles local profiles instantly.
3.  **GrooveStats Networking**
    *   **Function:** `GrooveStatsManager` singleton.
    *   **Status:** Integrated into GameLoop. Foundation for Score Submission/Leaderboards.
4.  **Gameplay Tweaks**
    *   **Rate Mods:** Pitch Dependent (Vinyl) vs Independent (Stretch) preference.
    *   **Mine Fix:** Logic corrected for non-held mines.
    *   **Visual Delay:** Per-player audio sync.
    *   **Ghost Tapping:** Option to disable Ghost Tapping penalty.

### B. NotITG Parity (Visual Modding)
1.  **Advanced Shaders**
    *   **Function:** `Actor:SetShader(vert, frag)`, `Actor:SetUniform(name, val)`.
    *   **Details:** Full GLSL support exposed to Lua.
2.  **Lua Hooks**
    *   **Function:** `Actor:SetDrawFunction(func)`, `Actor:SetUpdateFunction(func)`.
    *   **Details:** Allows overriding C++ rendering logic from Lua.
3.  **Deep Modding Primitives**
    *   **ActorProxy:** Verified `SetTarget` support for efficient duplication.
    *   **ActorMultiVertex:** Verified `SetDrawState` (Primitive control), `SetSpline`, `SetState` (Animation), and `SetVertex` tables.
    *   **ActorFrameTexture:** Verified `EnablePreserveTexture` (Feedback loops) and `SetTextureName` (Shader binding).
4.  **NotePath Actor**
    *   **Details:** New Actor type to visualize arrow trajectories (Modfile requirement).
5.  **Visual Control**
    *   **Wireframe:** `Actor:SetPolygonMode`.
    *   **Perspective:** `FOV` and `VanishY` player options.
6.  **Window Manipulation**
    *   **Function:** `DISPLAY:SetWindowPosition(x, y)`, `SetWindowSize(w, h)`.
    *   **Details:** X11 implementation included.

### C. Etterna / OutFox Parity (Universal QoL)
1.  **Discord Rich Presence (RPC)**
    *   **Function:** `DISCORD` singleton.
    *   **Lua Bindings:** `DISCORD:Initialize()`, `DISCORD:SetPresence(details, state, imageKey)`.
    *   **Status:** Stub implementation ready for library integration (avoids build breakage). Integrated into GameLoop.
2.  **Toast System (Notifications)**
    *   **Function:** `SCREENMAN:ToastMessage(str)`.
    *   **Usage:** Non-intrusive system notifications (e.g., "Score Saved", "Online").
3.  **Sandboxed File I/O**
    *   **Namespace:** `File`.
    *   **Functions:** `File.Read(path)`, `File.Write(path, content)`, `File.Append(path, content)`.
    *   **Security:** Writing/Appending is strictly limited to the `Save/` directory.

## 3. Economy & Token Foundation
*   **EconomyManager:** Singleton `ECONOMYMAN` for "Tip Economy" logic.
*   **Persistence:** Saves wallet state to `Save/Economy.xml`.

## 4. Build System Updates
*   Added `src/Discord/DiscordManager.cpp` to `CMakeData-singletons.cmake`.
*   Added `src/LuaModules/LuaModule_File.cpp` to `CMakeData-singletons.cmake`.
*   Added `src/NotePath.cpp`, `src/Economy/EconomyManager.cpp` previously.
*   Verified `ActorMultiVertex` and `ActorFrameTexture` are correctly registered.

## 5. Next Steps for Maintainers
1.  **Discord Library:** Add the `discord-rpc` library to `extern/` and update `DiscordManager.cpp` to call real functions instead of logging.
2.  **Theme Integration:** Update the default theme (`_fallback` or `Lambda`) to utilize `ToastMessage` and display `GrooveStats` data.
