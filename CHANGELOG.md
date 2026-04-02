# Unified StepMania Changelog

## [5.7.12-Unified-Beta-FinalHandoff] - 2026-02-10
### Changed
- **Master AI Handoff:** Overwrote the root `HANDOFF.md` with an encyclopedic, exhaustive summary of the Unified StepMania architecture (C++ Singletons + Lua 5.1 Sandbox). Detailed the exact state of all UI overlays, the Node.js mock server, the git feature branch merges, and the explicitly prioritized next steps for Gemini 3, Claude Opus 4.6, and GPT Codex 5.3 (e.g., Ghost Rendering, `libtorrent` swarms, BlueZ heart rate drivers, and Bobcoin JSON-RPC).

## [5.7.11-Unified-Beta-Matchmaker] - 2026-02-10
### Added
- **Node.js Matchmaking Server:** Completely rewrote `server/mock_server.js` from a simple chat echo server into a robust JSON WebSocket handler maintaining `matchmakingQueue` and `ACTIVE_MATCHES`.
- Overhauled `Docs/SERVER_SETUP.md`.

## [5.7.10-Unified-Beta-Handoff] - 2026-02-10
### Changed
- **Final Documentation Restructuring:** Rebuilt `Docs/ROADMAP.md`, `Docs/TODO.md`, `Docs/IDEAS.md`, and `Docs/SUBMODULES.md`.

## [5.7.9-Unified-Beta-CloudSync] - 2026-02-10
### Added
- **Cloud Profile Sync:** Revamped `ScreenAssetSync` matrix and `AssetSyncManager.cpp` API hooks (`PushToCloud` / `PullFromCloud`).

## [5.7.8-Unified-Beta-Merge] - 2026-02-10
### Added
- **Feature Branches:** Merged upstream branches `origin/x11_fullscreen` and `origin/merge-itgmania-features-3960352300100248355`.

## [5.7.7-Unified-Beta-Settings] - 2026-02-10
### Added
- **Unified Options Menu:** Created `ScreenUnifiedOptions`.
