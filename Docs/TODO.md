# Unified StepMania TODO.md

This document tracks immediate, actionable tasks and missing features identified during the deep architectural review.

## Immediate Backend Tasks (Phase 2 Priorities)

### 1. `EconomyManager` - Real Blockchain Hookup
- **Current State:** Hardcoded initial balance, XML parsing for transactions, static marketplace JSON.
- **Action:** Replace `BuyItem` and `AwardMiningReward` logic with HTTP RPC calls to a real Bobcoin node (using `extern/bobcoin` or generic `jsoncpp` requests). Remove the dependency on `Economy.xml.sig` in favor of trusting the ledger.

### 2. `HeartRateManager` - Hardware Drivers
- **Current State:** Returns a sine wave simulating 80-140 BPM to `ScreenGymWorkout`.
- **Action:** Implement an abstract `IHeartRateDriver`. Build a Windows implementation using WinRT Bluetooth APIs, and a Linux implementation using BlueZ. Make it poll asynchronously so it doesn't stutter the game loop.

### 3. `ContentSwarmManager` - BitTorrent Integration
- **Current State:** Uses legacy `FileDownload.cpp` to fetch an HTTP stub.
- **Action:** Replace `FileTransfer` with a lightweight libtorrent implementation. The UI should display seeds, leeches, and chunk progress instead of a generic "Downloading..." string.

### 4. `TournamentManager` - Matchmaking Server
- **Current State:** `mock_server.js` echoes chat. The Manager simulates an opponent finding and scoring.
- **Action:** Expand the Node.js backend to manage WebSockets (`ws`). Implement a matchmaking queue, ELO tracking via Redis or PostgreSQL, and real-time score streaming.

## Immediate UI/Frontend Tasks

### 1. Ghost Replay Rendering
- **Current State:** `ReplayManager` saves inputs to a CSV file. `ScreenReplayMenu` loads the mock list.
- **Action:** Modify `Player.cpp` to accept a `ReplayData` object. If present, instantiate a secondary `NoteField` that consumes those inputs exactly at their recorded timestamps, drawing a translucent "Ghost" player.

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

## Bugs & Edge Cases
- **Linux X11 Fullscreen:** *Status: Resolved in v5.7.8 via branch merge.*
- **AssetSyncManager Progress Bar:** The HTTP downloader doesn't always expose total file size, causing the progress bar in `ScreenAssetSync` to hang at 90% until completion. Must parse `Content-Length` headers more aggressively.
- **Discord RPC Crashes:** If the Discord desktop client crashes, the C++ `DiscordManager` might deadlock. Implement a timeout on the IPC pipe.
