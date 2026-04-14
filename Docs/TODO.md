# Unified StepMania TODO.md

This document tracks immediate, actionable tasks and missing features identified during the deep architectural review.

## Immediate Backend Tasks (Phase 2 Priorities)

### 1. `EconomyManager` - Real Blockchain Hookup
- **Current State:** Hardcoded initial balance, XML parsing for transactions, static marketplace JSON.
- **Action:** Replace `BuyItem` and `AwardMiningReward` logic with HTTP RPC calls to a real Bobcoin node (using `extern/bobcoin` or generic `jsoncpp` requests). Remove the dependency on `Economy.xml.sig` in favor of trusting the ledger.

### 2. `HeartRateManager` - Hardware Drivers
<<<<<<< HEAD
- **Current State:** The architectural base (IHeartRateDriver, HeartRateManager, and Mock driver) is completed.
- **Action:** Hook `HEARTRATEMAN->Update()` into `GameLoop::Update()`. Expose `HEARTRATEMAN:GetBPM()` to Lua via `LuaBinding`. Create actual hardware drivers (`BlueZ` / `WinRT`) to replace the Mock driver.
=======
- **Current State:** Returns a sine wave simulating 80-140 BPM to `ScreenGymWorkout`.
- **Action:** Implement an abstract `IHeartRateDriver`. Build a Windows implementation using WinRT Bluetooth APIs, and a Linux implementation using BlueZ. Make it poll asynchronously so it doesn't stutter the game loop.
>>>>>>> origin/unified-ui-features-13937230807013224518

### 3. `ContentSwarmManager` - BitTorrent Integration
- **Current State:** Uses legacy `FileDownload.cpp` to fetch an HTTP stub.
- **Action:** Replace `FileTransfer` with a lightweight libtorrent implementation. The UI should display seeds, leeches, and chunk progress instead of a generic "Downloading..." string.

### 4. `TournamentManager` - Matchmaking Server
<<<<<<< HEAD
- **Current State:** `mock_server.js` echoes chat. The Manager simulates an opponent finding and scoring.
- **Action:** Expand the Node.js backend to manage WebSockets (`ws`). Implement a matchmaking queue, ELO tracking via Redis or PostgreSQL, and real-time score streaming.
=======
- **Current State:** Node backend implements simple queueing and UUID mapping.
- **Action:** Expand the Node.js backend to handle PostgreSQL persistence for the ELO database to replace the in-memory array.
>>>>>>> origin/unified-ui-features-13937230807013224518

## Immediate UI/Frontend Tasks

### 1. Ghost Replay Rendering
<<<<<<< HEAD
- **Current State:** `ReplayManager` saves inputs to a CSV file. `ScreenReplayMenu` loads the mock list.
- **Action:** Modify `Player.cpp` to accept a `ReplayData` object. If present, instantiate a secondary `NoteField` that consumes those inputs exactly at their recorded timestamps, drawing a translucent "Ghost" player.
=======
- **Current State:** `ReplayManager` saves inputs to a CSV file. `ScreenReplayMenu` loads the mock list. `ReplayManager::GetPlaybackInputAtTime` exposes a clean array of inputs at any given timestamp.
- **Action:** Modify `Player.cpp` to query `REPLAYMAN->GetPlaybackInputAtTime()` on every tick. If present, instantiate a secondary `NoteField` that consumes those inputs exactly at their recorded timestamps, drawing a translucent "Ghost" player. *This is fully prepared for the implementor.*
>>>>>>> origin/unified-ui-features-13937230807013224518

### 2. Tournament Draft Phase Networking
- **Current State:** `ScreenTournamentDraft` is a beautiful Lua UI that simulates a CPU opponent taking turns to ban/pick charts.
- **Action:** Wire the draft screen to `UnifiedNetwork` so two human players actually see the opponent's cursor and selections in real-time.

### 3. Mission Engine Persistence
- **Current State:** `MissionManager` tracks NotesHit and Calories in-memory and saves to `Missions.xml`.
- **Action:** Connect `ClaimReward` to the Unified Server so users cannot manually edit their `Missions.xml` to farm infinite Bobcoin.

## Legacy Code Refactoring (Technical Debt)

### 1. `ScoreKeeperNormal` vs `WifeScoring`
- **Issue:** Etterna parity demands we use Wife3 scoring, but `ScoreKeeperNormal.cpp` is deeply entrenched with DDR-style logic (`GetScore`, `HandleRowComboInternal`).
- **Action:** Write a cleanly separated `ScoreKeeperUnified` that completely deprecates the old logic, implementing J1-J8 scaling natively without hacking it into `HandleTapScore`.

### 2. Remove `EzSockets` Dependency
- **Issue:** StepMania's legacy networking is notoriously crash-prone and blocks the main thread occasionally.
- **Action:** Rewrite `UnifiedNetwork` using modern C++11 `<thread>` and `<mutex>` alongside a robust library like `libuv` or `asio`.
