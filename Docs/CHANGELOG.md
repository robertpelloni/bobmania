# Changelog

## [5.7.5-Unified-Beta] - 2025-12-27
### Added
- **Documentation:** Sweeping updates to `VISION.md`, `DEPLOY.md`, `SUBMODULES.md`, and `LLM_UNIVERSAL.md`.
- **Documentation:** Consolidated all agent-specific instruction files (`AGENTS.md`, `CLAUDE.md`, etc.) to point to a single source of truth.
- **Backend:** (In Progress) Transitioning `EconomyManager` to use `BobcoinBridge` for real RPC integration.

## [5.7.4-Unified-Beta] - 2025-12-27
### Added
- **Networking:** Implemented `ContentSwarmManager` singleton stub that mimics a `libtorrent` session.
- **Lua Bindings:** Exposed `SWARMMAN` methods (`GetDownloadProgress`, `GetSeedCount`, etc.) to frontend via `Luna<T>`.
- **UI:** Added `ScreenContentNetwork overlay.lua` to visually track P2P swarm progress.

## [5.7.3-Unified-Beta] - 2026-04-14
### Added
- **Gym Mode:** Complete Lua bindings for `HEARTRATEMAN`.
- **UI:** Added `ScreenGymWorkout overlay.lua` which visualizes the player's live heart rate.
- **Documentation:** Documented missing C++ engine dependencies (`StepMania.cpp`, `GameLoop.cpp`) for upstream consumers.

## [5.7.2-Unified-Beta] - 2026-04-11
### Fixed
- **CI/CD:** Upgraded deprecated `macos-13` GitHub Actions runners to `macos-14`.
- **CI/CD:** Made `xmllint` execution graceful when Lua documentation files are unbuilt.
- **Repository:** Systematically resolved over 90 scattered Git merge conflict markers (`<<<<<<< HEAD`) caused by multi-fork integrations.

## [5.7.1-Unified-Alpha] - 2026-04-10
### Added
- **Hardware:** Added `IHeartRateDriver` and `HeartRateDriver_Mock` for Gym mode heart rate monitoring.
- **Hardware:** Added `HeartRateManager` singleton to poll drivers and update `GYMMAN`.
- **Backend:** Connected `EconomyManager` to Bobcoin RPC node stubs.
### Changed
- **Documentation:** Massively updated `VISION.md`, `ROADMAP.md`, `TODO.md`, `HANDOFF.md`, and `UNIVERSAL_LLM_INSTRUCTIONS.md` to guide future development.
## [v5.71.0] - Unified-Beta Phase 5 Prep
- Resolved Phase 5 immediate action items from TODO.md.
- Verified NoteField:443 rainbow mode support is already completed.
- Implemented `TapNoteSubType_Checkpoint` for checkpoint holds in NoteField and NoteTypes.
- Removed legacy `StdString.h` requirement globally (CMake lists & project files).
- Exposed `BothAtOnce` explicitly via the Lua UI `ScreenUnifiedOptions` (mock and hook).
