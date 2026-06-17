# Unified StepMania: Foundation Milestone 2 Handoff

**Status:** Milestone 2 (Economy & Connected Features) COMPLETED.
**Build:** 5.7.0-Unified-Alpha-v4
**Next Milestone:** Milestone 3 (Tournament Mode & Advanced Networking)

## Summary of Completed Features
1.  **Economy & Play-to-Earn:**
    - Real `BobcoinBridge` (JSON-RPC via EzSockets/JsonCpp) replaces all mocks.
    - `EconomyManager::AwardCompletionRewards` implements performance-based Bobcoin mining using the Wife3 metric.
    - Real-time marketplace item fetching and purchase logic.
2.  **Infrastructure & Architecture:**
    - All Singleton Managers refactored to use `std::unique_ptr` for safety and modern standards.
    - Centralized versioning in `VERSION.md` with a C++ loader (`src/ver.cpp`).
    - OS-specific Heart Rate Driver framework (`WinRT` for Windows, `BlueZ` for Linux).
3.  **Engine Parity Fixes:**
    - **Mine Fix:** DinsFire64 community fix for consistent triggering.
    - **Phantom Hold Fix:** FSX fix for preventing hold failure on passed rows.
4.  **UI & Visuals:**
    - **Ghost NoteField:** Implemented transparent replay visualization in the `Player` actor.
    - **Swarm Network:** Added "Seed" functionality to share local packs.
    - **Asset Sync:** Real-time progress and status reporting.

## Technical Notes
- **JSON-RPC:** Nodes are configured via `BobcoinNodeIP` and `BobcoinNodePort` preferences in `Data/Static.ini`.
- **VFS Versioning:** Version string is read via `RageFile` during early startup (`InitProductVersion` in `sm_main`).
- **Memory:** Use `std::make_unique` for all new manager sub-components.

## Next Steps (Milestone 3)
- Visual bracket rendering for `ScreenTournamentLadder`.
- Real `libtorrent` integration for the Content Swarm.
- Transition network calls to a non-blocking async architecture.
- Implement more mission types (e.g., ELO-based goals).
