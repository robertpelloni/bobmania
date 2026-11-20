# Changelog

## [5.172.0] - 2026-11-04
### Added / Changed
- **NotITG Spline Editor:** Integrated `ScreenSplineEditor overlay.lua` for chart authors to visually inspect and modify 3D NotePath spline control points.
- **Metrics & Theme Registration:** Added `[ScreenSplineEditor]` metric definitions to `Themes/default/metrics.ini` and header strings in `Themes/default/Languages/en.ini`.
- **Test Automation:** Added automated Lua syntax verification for `ScreenSplineEditor overlay.lua` to `src/tests/run_tests.sh`.

## [5.171.0] - 2026-11-04
### Added / Changed
- **Dashboard Consolidation:** Re-architected `ScreenUnifiedDashboard` into a high-value, single-page command center.
- **Categorization & Layout:** Grouped all 13 modules by high-value prominent placement:
  * Core Rhythm & Fitness: Arcade Mode (`ScreenSelectMusic`), Gym & Fitness (`ScreenGymWelcome`), Tournament Ladder (`ScreenTournamentLadder`), Online NetRoom (`ScreenNetRoom`).
  * Economy & Progression: Marketplace (`ScreenMarketplace`), Wallet Ledger (`ScreenWalletHistory`), Missions & Quests (`ScreenMissionSelect`), Cloud Asset Sync (`ScreenAssetSync`).
  * Media & System Tools: Spectator TV (`ScreenSpectate`), Replay Theater (`ScreenReplayMenu`), P2P Content Swarm (`ScreenContentNetwork`), Unified Settings (`ScreenUnifiedOptions`), Exit / Server Shutdown (`ScreenExit`).
- **Context Guidance:** Built-in category tags, prominent badges, and contextual tooltip bar driven by `HelpOverlay`.
- **Documentation:** Synchronized `Docs/DASHBOARD.md` and `Docs/Manual_Unified.md`.
- **Testing:** Verified 100% test pass on `src/tests/run_tests.sh`.

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
