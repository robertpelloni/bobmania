# Unified StepMania TODO.md

This document tracks immediate, actionable tasks and missing features identified during the deep architectural review.

## Immediate Backend Tasks (Phase 2 Priorities)

### 1. `EconomyManager` - Real Blockchain Hookup
- **Current State:** Completed.
- **Action:** Replace `BuyItem` and `AwardMiningReward` logic with HTTP RPC calls to a real Bobcoin node (using `extern/bobcoin` or generic `jsoncpp` requests). Remove the dependency on `Economy.xml.sig` in favor of trusting the ledger.

### 2. `HeartRateManager` - Hardware Drivers
- **Current State:** Completed.
- **Action:** Hook `HEARTRATEMAN->Update()` into `GameLoop::Update()`. Expose `HEARTRATEMAN:GetBPM()` to Lua via `LuaBinding`. Create actual hardware drivers (`BlueZ` / `WinRT`) to replace the Mock driver.

### 3. `ContentSwarmManager` - BitTorrent Integration
- **Current State:** Completed.
- **Action:** Replace `FileTransfer` with a lightweight libtorrent implementation. The UI should display seeds, leeches, and chunk progress instead of a generic "Downloading..." string.

### 4. `TournamentManager` - Matchmaking Server
- **Current State:** Completed. Expanded WebSockets and Elo logic.
- **Action:** Expand the Node.js backend to manage WebSockets (`ws`). Implement a matchmaking queue, ELO tracking via Redis or PostgreSQL, and real-time score streaming.

## Immediate UI/Frontend Tasks

### 1. Ghost Replay Rendering
- **Current State:** Completed.
- **Action:** Modify `Player.cpp` to accept a `ReplayData` object. If present, instantiate a secondary `NoteField` that consumes those inputs exactly at their recorded timestamps, drawing a translucent "Ghost" player.

### 2. Tournament Draft Phase Networking
- **Current State:** Completed.
- **Action:** Wire the draft screen to `UnifiedNetwork` so two human players actually see the opponent's cursor and selections in real-time.

### 3. Mission Engine Persistence
- **Current State:** Completed.
- **Action:** Connect `ClaimReward` to the Unified Server so users cannot manually edit their `Missions.xml` to farm infinite Bobcoin.

## Legacy Code Refactoring (Technical Debt)

### 1. `ScoreKeeperNormal` vs `WifeScoring`
- **Issue:** Completed. `ScoreKeeperUnified` implemented natively.
- **Action:** Write a cleanly separated `ScoreKeeperUnified` that completely deprecates the old logic, implementing J1-J8 scaling natively without hacking it into `HandleTapScore`.

### 2. Remove `EzSockets` Dependency
- **Issue:** Completed. Replaced with `UnifiedNetwork`.
- **Action:** Rewrite `UnifiedNetwork` using modern C++11 `<thread>` and `<mutex>` alongside a robust library like `libuv` or `asio`.
