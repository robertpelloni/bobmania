# Changelog

## [5.7.1-Unified-Beta] - 2026-02-09

### Added
- **Economy:**
  - **Marketplace UI:** Full purchasing system with balance checks and "Already Owned" logic.
  - **Wallet History:** Transaction log display.
  - **Persistence:** Marketplace catalog loads from JSON with fallback.
- **Gym Mode:**
  - **Persistence:** Workout history and profile stats save to `Save/Gym.xml`.
  - **Dynamic Playlists:** `GymPlaylistGenerator` creates workouts based on duration and difficulty.
  - **UI:** `ScreenGymWorkout` (Live stats) and `ScreenGymSummary` (Post-workout report).
- **Network:**
  - **Spectator Mode:** Backend stub `SpectatorManager` and `ScreenSpectate` UI.
  - **Asset Sync:** `ScreenAssetSync` overlay for cross-platform data synchronization.
- **Replay System:**
  - **Browser:** `ScreenReplayMenu` for reviewing past plays.
  - **Engine:** Wired `ReplayManager` to `ScreenGameplay` for input recording.

### Changed
- **Documentation:** Major updates to `Docs/Manual_Unified.md` and `docs/ROADMAP.md`.
- **Help System:** `HelpOverlay` now covers all new screens.

## [5.7.0-Unified-Alpha] - 2026-02-08

### Added
- **Unified Dashboard:** Central hub for all game modes.
- **Economy:** Full wallet, marketplace, and mining system.
- **Fitness:** Gym mode with weight tracking and workout logs.
- **Competitive:** Tournament ladder and match scheduling.
- **Missions:** Dynamic mission system with progression tracking.
- **Documentation:** Comprehensive Vision, Manual, and Agent Instructions.

### Changed
- **Gameplay:** Added mining rewards hook on stage finish.
- **Options:** Added 'Judge Scale' modifier (Etterna-style).
- **Structure:** Added `Simply-Love-SM5` submodule and mocked `bobcoin` dependency.

## [5.1.1] - 2025-12-25

### Added
- **Economy UI:** Added wallet balance display to `ScreenGameplay`.
- **Dashboard Access:** Added "Dashboard" option to `ScreenTitleMenu`.
- **Documentation:** Added `docs/DASHBOARD.md` with project structure and submodule status.
- **LLM Instructions:** Consolidated all model instructions into `LLM_INSTRUCTIONS.md`.

### Changed
- **Versioning:** Enforced strict versioning rules in `LLM_INSTRUCTIONS.md`.
- **Dashboard:** Updated `ScreenUnifiedDashboard` to link to `ScreenWalletHistory`.

## [5.1.0] - 2025-12-25

### Added
- **Economy System:** Implemented `EconomyManager` and related classes.
- **Tournament Mode:** Implemented `ScreenLoginQR` and tournament features.
- **Unified Dashboard:** Added `ScreenUnifiedDashboard`.
- **Gym Mode:** Added `ScreenGymWelcome`.
- **Documentation:** Added `docs/DASHBOARD.md` and `LLM_INSTRUCTIONS.md`.

### Changed
- Merged `economy-tournament-mvp` and `itgmania-features` branches into `5_1-new`.
- Updated `ProfileManager` to support new economy features.
- Updated `ScreenGameplay` to integrate with new systems.
- Consolidated LLM instructions.

### Fixed
- Resolved merge conflicts in `ProfileManager.cpp`, `ScreenGameplay.cpp`, and others.
