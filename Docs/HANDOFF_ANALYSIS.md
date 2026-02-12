# Unified StepMania: Deep Analysis & Handoff Strategy

**Date:** 2026-02-09
**Version:** 5.7.1-Unified-Beta
**Author:** Jules (AI Developer)

## 1. Executive Summary
This project aims to unify the disparate StepMania forks (5.x, OutFox, NotITG, Etterna) into a single, feature-rich platform. We have successfully implemented the core infrastructure for the new "Unified" features: Economy, Gym, Tournament, and Network. The frontend is largely wired up, and the backend logic is present but relies on mocks for complex external systems (Blockchain, P2P Networking).

## 2. Architectural Overview

### 2.1 The "Manager" Pattern
We have standardized on a Singleton Manager pattern for new subsystems. These live in `src/` subdirectories and are globally accessible via pointers defined in `src/global.h` (e.g., `ECONOMYMAN`, `GYMMAN`).

*   **`EconomyManager` (`src/Economy/`)**: Handles the Ledger, Marketplace, and Mining.
    *   *Status:* Functional. Uses `Save/Economy.xml` for persistence.
    *   *Gap:* Currently uses a mock ledger. Needs connection to `extern/bobcoin`.
*   **`GymManager` (`src/Gym/`)**: Handles User Profile (Weight, BMI), Workout History, and Goal Tracking.
    *   *Status:* Functional. Uses `Save/Gym.xml`.
    *   *Gap:* Calorie formula is basic. Needs better biometric integration.
*   **`TournamentManager` (`src/Tournament/`)**: Handles Ladder logic, Matchmaking, and Results.
    *   *Status:* Logic implemented. Match execution simulates result.
    *   *Gap:* No real networking. "Opponents" are currently bots/mocks.
*   **`SpectatorManager` (`src/Network/`)**: Handles video/stream connection state.
    *   *Status:* Stubbed. UI exists.
    *   *Gap:* Needs `ffmpeg` or `librtmp` integration for real streaming.

### 2.2 The UI/Overlay System
We utilize StepMania's Lua scripting engine (`Lua 5.1`) to build modern UIs on top of the legacy engine.
*   **Screens:** New features have dedicated screens (e.g., `ScreenGymWorkout`) or Overlays (e.g., `ScreenMarketplace overlay.lua`).
*   **Bindings:** C++ Managers expose methods to Lua (e.g., `ECONOMYMAN:BuyItem(id)`).
*   **Help System:** A global `HelpOverlay.lua` provides context-sensitive tooltips.

## 3. Module Status & Deep Dive

### 3.1 Economy Module
*   **Implemented:**
    *   Wallet balance tracking.
    *   Transaction history logging.
    *   Marketplace catalog (loaded from JSON).
    *   "Mining" reward hook in `ScreenGameplay`.
*   **Missing:**
    *   **True Blockchain:** The `BobcoinBridge` is needed. `extern/bobcoin` is a submodule but not linked.
    *   **Security:** `Save/Economy.xml` is plaintext. Needs `CryptManager` signing.

### 3.2 Gym Module
*   **Implemented:**
    *   Profile creation/editing.
    *   BMI calculation.
    *   Dynamic Playlist Generation (`GymPlaylistGenerator`) based on song duration/difficulty.
*   **Missing:**
    *   **Heart Rate Monitor:** Support for ANT+/Bluetooth HRMs.
    *   **Advanced Metrics:** METs (Metabolic Equivalent of Task) calculation.

### 3.3 Tournament Module
*   **Implemented:**
    *   Ladder visualization.
    *   Match start logic (transitions to Gameplay).
    *   Result reporting (User Score vs. Simulated Opponent).
*   **Missing:**
    *   **Real Opponents:** Requires a server (WebSocket/IRC) to pair players.
    *   **Netplay:** Synchronization of gameplay (like SMO/EtternaOnline).

## 4. Path Forward (Recommendations for Next Models)

### 4.1 Phase 1: Real Connectivity
1.  **Implement `BobcoinBridge`:**
    *   Create an interface in `src/Economy/Bridge/`.
    *   Link against `extern/bobcoin/lib/libbobcoin.a`.
    *   Replace `m_iBalance` manipulation with calls to the library.
2.  **Implement `NetworkClient`:**
    *   Replace the mock `TournamentManager` logic with real server messages.
    *   Use `ezsockets` or `websocketpp` (if added) for communication.

### 4.2 Phase 2: Polish & Security
1.  **Encrypt Save Data:** Use `CryptManager` to sign/encrypt `Gym.xml` and `Economy.xml`.
2.  **Asset Sync:** Actually download files in `AssetSyncManager`.

### 4.3 Phase 3: Content
1.  **Marketplace Content:** Create real content packs (Songs, Themes) to populate the JSON catalog.
2.  **Gym Workouts:** Create refined difficulty curves for generated playlists.

## 5. Submodule Strategy
*   **`extern/bobcoin`**: Ensure this is built as a static library by CMake.
*   **`extern/ffmpeg`**: Only link if `WITH_FFMPEG` is set to avoid bloat.

## 6. Development Directives
*   **Preserve Legacy:** Do not break existing SM5 themes.
*   **Documentation First:** Update `Manual_Unified.md` before writing code.
*   **UI Coverage:** Every C++ feature *must* have a Lua binding and a UI element.

**Signed:** Jules
