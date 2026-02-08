# Changelog

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

## [5.7.0-Unified-Alpha] - 2026-02-08
### Added
- **Unified Dashboard:** Central hub for all game modes.
- **Economy:** Full wallet, marketplace, and mining system ().
- **Fitness:** Gym mode with weight tracking and workout logs ().
- **Competitive:** Tournament ladder and match scheduling ().
- **Missions:** Dynamic mission system with progression tracking ().
- **Documentation:** Comprehensive Vision, Manual, and Agent Instructions.

### Changed
- **Gameplay:** Added mining rewards hook on stage finish.
- **Options:** Added 'Judge Scale' modifier (Etterna-style).
- **Structure:** Added `Simply-Love-SM5` submodule and mocked `bobcoin` dependency.
