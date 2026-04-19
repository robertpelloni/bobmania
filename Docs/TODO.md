# Unified StepMania TODO.md

This document tracks the next generation of immediate, actionable tasks and missing features identified after the successful completion of the Phase 1-4 architectural prototypes.

## Immediate Backend Tasks (Phase 5 Prep)

### 1. `libuv` / `asio` Integration for `UnifiedNetwork`
- **Current State:** `UnifiedNetwork` provides a thread-safe mutex wrapper but lacks high-performance asynchronous non-blocking event loops.
- **Action:** Replace the mocked C++ `<thread>` socket logic with `libuv` or Boost.Asio. Ensure `FileDownload.cpp` can fetch `.smzip` assets over HTTPS asynchronously without stalling the main `GameLoop`.

### 2. Live Database for `mock_server.js`
- **Current State:** The Matchmaking and Elo system (`server/mock_server.js`) currently tracks users and scores in memory via Node.js `Map`s.
- **Action:** Introduce PostgreSQL via `pg` or `Sequelize` to the backend. Persist User accounts, Elo rankings, and Chat History permanently. Provide an endpoint for the `TournamentManager` to query global leaderboards.

### 3. OpenVR / Oculus SDK Dynamic Loading
- **Current State:** `ArchHooks_VR` provides the stereoscopic matrix extraction prototypes, and `RageDisplay` splits the render loop.
- **Action:** Implement `LoadLibrary`/`dlopen` specifically within `ArchHooks_VR` to load `openvr_api.dll` or `OVRPlugin.dll` at runtime. Do not statically link them to avoid breaking compilation on systems without VR headsets.

## Immediate UI/Frontend Tasks

### 1. Unified Leaderboard UI
- **Current State:** `ScreenTournamentLadder` exists but is largely a static visual mock.
- **Action:** Create `Themes/default/Scripts/15_Leaderboard.lua`. Expose a method in `TournamentManager` that performs an HTTP GET to the Node.js backend to fetch the Top 100 Elo players. Render this dynamically in the UI.

### 2. NotITG Spline Editor (Debug Mode)
- **Current State:** `NotePath` spline mathematics work in C++, and can be manipulated via Lua `13_NotePath.lua`.
- **Action:** Build a debug UI (`ScreenSplineEditor overlay.lua`) that allows chart authors to drag control points with the mouse and instantly see the 3D spline update, exporting the raw coordinates to their `.sm` file.

## Legacy Code Refactoring (Technical Debt)

### 1. Remove `RageFile` XML Dependencies
- **Issue:** `XmlFile` is notoriously slow and brittle. Modern modding and web services use JSON.
- **Action:** Systematically replace all instances of `XmlFile.Load()` (such as in `Profile.cpp` and `Missions.xml`) with `jsoncpp`. Profiles should be saved and loaded strictly as JSON objects to ensure frictionless syncing with the Node.js backend.
