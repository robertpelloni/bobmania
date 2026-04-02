# Unified StepMania Changelog

## [5.7.13-Unified-Beta-GhostEngine] - 2026-02-10
### Added
- **Ghost Player Backend:** Significantly refactored `ReplayManager.cpp` to expose `GetPlaybackInputAtTime()`. This provides a clean interface for `Player.cpp` to inject Ghost Replay data deterministically, solving the largest remaining C++ architectural hurdle outlined in `TODO.md`.

## [5.7.12-Unified-Beta-FinalHandoff] - 2026-02-10
### Changed
- **Master AI Handoff:** Overwrote the root `HANDOFF.md` with an encyclopedic, exhaustive summary of the Unified StepMania architecture.

## [5.7.11-Unified-Beta-Matchmaker] - 2026-02-10
### Added
- **Node.js Matchmaking Server:** Completely rewrote `server/mock_server.js`.

## [5.7.10-Unified-Beta-Handoff] - 2026-02-10
### Changed
- **Final Documentation Restructuring:** Rebuilt `Docs/ROADMAP.md`, `Docs/TODO.md`, `Docs/IDEAS.md`, and `Docs/SUBMODULES.md`.

## [5.7.9-Unified-Beta-CloudSync] - 2026-02-10
### Added
- **Cloud Profile Sync:** Revamped `ScreenAssetSync`.

## [5.7.8-Unified-Beta-Merge] - 2026-02-10
### Added
- **Feature Branches:** Merged upstream branches.

## [5.7.7-Unified-Beta-Settings] - 2026-02-10
### Added
- **Unified Options Menu:** Created `ScreenUnifiedOptions`.

## [5.7.6-Unified-Beta-Discord] - 2026-02-10
### Added
- **Discord Rich Presence:** Created `Themes/default/Scripts/11 Discord.lua`.

## [5.7.5-Unified-Beta-EngineStubs] - 2026-02-10
### Added
- **ReplayManager:** Implemented the core engine singleton.

## [5.7.4-Unified-Beta-MissionEngine] - 2026-02-10
### Added
- **Mission Engine:** Implemented logic within `MissionManager.cpp`.

## [5.7.3-Unified-Beta-Documentation] - 2026-02-10
### Added
- Created `Docs/VISION.md`, `Docs/SUBMODULES.md`, `Docs/TODO.md`, `Docs/ROADMAP.md`, `Docs/LLM_UNIVERSAL.md`.
