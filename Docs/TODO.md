# Unified StepMania TODO.md

This document tracks the next generation of immediate, actionable tasks and missing features identified after the successful completion of the Phase 1-4 architectural prototypes.

## Immediate Backend Tasks (Phase 5 Prep)

### 1. `libuv` / `asio` Integration for `UnifiedNetwork`
- **Current State:** `UnifiedNetwork` provides a thread-safe mutex wrapper but lacks high-performance asynchronous non-blocking event loops.
- **Action:** Replace the mocked C++ `<thread>` socket logic with `libuv` or Boost.Asio. Ensure `FileDownload.cpp` can fetch `.smzip` assets over HTTPS asynchronously without stalling the main `GameLoop`.

### 2. `HeartRateManager` - Hardware Drivers
- **Current State:** The architectural base (IHeartRateDriver, HeartRateManager, and Mock driver) is completed.
- **Action:** Hook `HEARTRATEMAN->Update()` into `GameLoop::Update()`. Expose `HEARTRATEMAN:GetBPM()` to Lua via `LuaBinding`. Create actual hardware drivers (`BlueZ` / `WinRT`) to replace the Mock driver.
- **Current State:** Returns a sine wave simulating 80-140 BPM to `ScreenGymWorkout`.
- **Action:** Implement an abstract `IHeartRateDriver`. Build a Windows implementation using WinRT Bluetooth APIs, and a Linux implementation using BlueZ. Make it poll asynchronously so it doesn't stutter the game loop.

### 3. `ContentSwarmManager` - BitTorrent Integration
- **Current State:** Uses legacy `FileDownload.cpp` to fetch an HTTP stub.
- **Action:** Replace `FileTransfer` with a lightweight libtorrent implementation. The UI should display seeds, leeches, and chunk progress instead of a generic "Downloading..." string.

### 4. `TournamentManager` - Matchmaking Server
- **Current State:** `mock_server.js` echoes chat. The Manager simulates an opponent finding and scoring.
- **Action:** Expand the Node.js backend to manage WebSockets (`ws`). Implement a matchmaking queue, ELO tracking via Redis or PostgreSQL, and real-time score streaming.
- **Current State:** Node backend implements simple queueing and UUID mapping.
- **Action:** Expand the Node.js backend to handle PostgreSQL persistence for the ELO database to replace the in-memory array.

## Immediate UI/Frontend Tasks

### 1. Ghost Replay Rendering
- **Current State:** `ReplayManager` saves inputs to a CSV file. `ScreenReplayMenu` loads the mock list. `ReplayManager::GetPlaybackInputAtTime` exposes a clean array of inputs at any given timestamp. `Player.cpp` correctly polls this during playback and draws a translucent ghost `NoteField`. (Completed by Jules).
- **Action:** Refine the rendering to support specialized NoteSkins for ghost elements, and integrate ghost selection directly into the evaluation and pre-game lobbies.

### 2. NotITG Spline Editor (Debug Mode)
- **Current State:** `NotePath` spline mathematics work in C++, and can be manipulated via Lua `13_NotePath.lua`.
- **Action:** Build a debug UI (`ScreenSplineEditor overlay.lua`) that allows chart authors to drag control points with the mouse and instantly see the 3D spline update, exporting the raw coordinates to their `.sm` file.

## Legacy Code Refactoring (Technical Debt)

### 1. Remove `RageFile` XML Dependencies
- **Issue:** `XmlFile` is notoriously slow and brittle. Modern modding and web services use JSON.
- **Action:** Systematically replace all instances of `XmlFile.Load()` (such as in `Profile.cpp` and `Missions.xml`) with `jsoncpp`. Profiles should be saved and loaded strictly as JSON objects to ensure frictionless syncing with the Node.js backend.
