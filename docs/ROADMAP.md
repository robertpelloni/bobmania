# Unified StepMania Project Roadmap

This document tracks the implementation status of all features required for the "Unified StepMania" vision.

## 1. Core Systems (Backend)

### Economy (EconomyManager)
- [x] Basic Ledger (Balance, Transactions).
- [x] Mining Simulation (`AwardMiningReward`).
- [x] Marketplace Purchasing (`BuyItem`).
- [x] Persistence (`Save/Economy.xml`).
- [x] **Dynamic Catalog:** Load items from `Data/MarketplaceCatalog.json` (fallback implemented).
- [ ] **Real Blockchain Bridge:** Replace mock with `extern/bobcoin` calls.

### Gym (GymManager)
- [x] Profile Management (Weight, BMI, Goals).
- [x] Lua Bindings (`LogWorkout`, `GetProfile`).
- [x] **Persistence:** Save/Load `WorkoutHistory` to `Save/Gym.xml`.
- [x] **Playlist Generator:** `GymPlaylistGenerator` creates workouts based on intensity/length.

### Tournament (TournamentManager)
- [x] Ladder Display (Mock Data).
- [x] Upcoming Matches (Mock Data).
- [ ] **Match Execution:** Implement `StartMatch` to set up `GAMESTATE` for a specific song/mod set.
- [ ] **Result Reporting:** Implement `ReportMatchResult` to update ELO.

### Scoring & Replay (Unified)
- [x] **Wife Scoring:** Implemented J4 scoring curve (`Scoring/WifeScoring.cpp`).
- [x] **Replay Backend:** `ReplayManager` wired to Gameplay loop.
- [x] **Replay UI:** `ScreenReplayMenu` for browsing/watching.

### Network (Unified)
- [x] **Spectator Backend:** `SpectatorManager` stub implemented.
- [x] **Spectator UI:** `ScreenSpectate` overlay.
- [x] **Asset Sync:** `ScreenAssetSync` overlay.

## 2. User Experience (Frontend)

### Dashboard
- [x] Unified Dashboard Screen.
- [x] Help Overlay Integration (Global).

### Gym UI
- [x] Welcome Screen (`ScreenGymWelcome`).
- [x] Workout Screen (`ScreenGymWorkout`).
- [x] Workout Summary (`ScreenGymSummary`).
- [x] Help Integration.

### Tournament UI
- [x] Ladder Screen (`ScreenTournamentLadder`).
- [x] Spectator Integration (`ScreenSpectate`).

### Economy UI
- [x] Marketplace (`ScreenMarketplace`).
- [x] Wallet History (`ScreenWalletHistory`).

## 3. Immediate Priorities (Next Steps)

1.  **Tournament Match Logic:** Allow players to actually "play" a tournament match.
2.  **Real Blockchain:** Connect `BobcoinBridge`.
3.  **Code Cleanup:** Remove legacy comments and unused files.
