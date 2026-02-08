# Unified StepMania Project Roadmap

This document tracks the implementation status of all features required for the "Unified StepMania" vision.

## 1. Core Systems (Backend)

### Economy (EconomyManager)
- [x] Basic Ledger (Balance, Transactions).
- [x] Mining Simulation (`AwardMiningReward`).
- [x] Marketplace Purchasing (`BuyItem`).
- [x] Persistence (`Save/Economy.xml`).
- [ ] **Dynamic Catalog:** Load items from `Data/MarketplaceCatalog.json` instead of hardcoding.
- [ ] **Real Blockchain Bridge:** Replace mock with `extern/bobcoin` calls.

### Gym (GymManager)
- [x] Profile Management (Weight, BMI, Goals).
- [x] Lua Bindings (`LogWorkout`, `GetProfile`).
- [ ] **Persistence:** Save/Load `WorkoutHistory` to disk.
- [ ] **Playlist Generator:** Implement `GymPlaylistGenerator` to create courses based on intensity/length.
- [ ] **Course Integration:** Populate `GAMESTATE->m_pCurCourse` dynamically.

### Tournament (TournamentManager)
- [x] Ladder Display (Mock Data).
- [x] Upcoming Matches (Mock Data).
- [ ] **Match Execution:** Implement `StartMatch` to set up `GAMESTATE` for a specific song/mod set.
- [ ] **Result Reporting:** Implement `ReportMatchResult` to update ELO.
- [ ] **Persistence:** Save/Load Match History and dynamic Ladder state.

### Scoring (Unified)
- [x] **Wife Scoring:** Implemented J4 scoring curve (`Scoring/WifeScoring.cpp`).
- [x] **MSD Calculation:** Implemented MVP.
- [ ] **Replay System:** Record and playback inputs (NotITG/Etterna parity).

## 2. User Experience (Frontend)

### Dashboard
- [x] Unified Dashboard Screen.
- [x] Help Overlay Integration.

### Gym UI
- [x] Welcome Screen (`ScreenGymWelcome`).
- [x] Workout Screen (`ScreenGymWorkout`).
- [ ] **Comprehensive Help:** Add `HelpOverlay` tooltips to these screens.
- [ ] **Workout Summary:** A screen to show stats after a set.

### Tournament UI
- [x] Ladder Screen (`ScreenTournamentLadder`).
- [ ] **Match Lobby:** A screen to wait for opponent/ready up.
- [ ] **Spectator View:** UI for watching a match (using `SpectatorManager`).

### Economy UI
- [x] Marketplace (`ScreenMarketplace`).
- [x] Wallet History (`ScreenWalletHistory`).
- [ ] **Transaction Details:** Click a transaction to see more info.

## 3. Immediate Priorities (Next Steps)

1.  **Gym Persistence:** Ensure workout history is saved.
2.  **Gym Playlist Generator:** Create the logic to build workouts.
3.  **Tournament Match Logic:** Allow players to actually "play" a tournament match.
4.  **UI Polish:** Apply `HelpOverlay` to all new screens.
