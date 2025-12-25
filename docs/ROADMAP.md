# StepMania Network: Project Roadmap & Status

## Overview
This document tracks the implementation status of the "StepMania Network" features, integrating concepts from various forks (Etterna, OutFox, StepManiaX) into a unified economy and tournament platform.

## Accomplished Features

### 1. Economy & Cryptocurrency (Simulated)
*   **Status:** Complete
*   **Key Components:**
    *   `EconomyManager`: Singleton managing a simulated blockchain ledger ("Save/Economy.ini").
    *   **Wallets:** Support for `WALLET_PLAYER`, `WALLET_HOUSE`, `WALLET_DAO`, and `ARTIST_` wallets.
    *   **Transactions:** `Transfer()` logic with logging and persistence.
    *   **Betting:** `ScreenBettingEntry` for placing wagers; `ScreenEvaluation` resolution logic.
    *   **Mining:** Passive income simulation for "Server Node" uptime.
    *   **Dividends:** Shareholders (DAO) receive periodic rewards based on Treasury balance.

### 2. Tournament System
*   **Status:** Complete
*   **Key Components:**
    *   `ScreenTournamentLadder`: Displays Elo rankings and Divisions (Pro/Gold/Silver).
    *   **Ranking:** Elo calculation logic in `EconomyManager`.
    *   **Brackets:** `TournamentBracket` class for visualizing match trees.
    *   **Rewards:** One-time bonuses for reaching Elo thresholds (Silver/Gold/Pro).

### 3. Governance (DAO)
*   **Status:** Complete
*   **Key Components:**
    *   `ScreenGovernance`: Interface for voting on proposals.
    *   **Weighted Voting:** Vote power determined by "Company Share" asset ownership.
    *   **Treasury:** Visualizes DAO funds accumulated from fees/sales.

### 4. Gym & Fitness
*   **Status:** Complete
*   **Key Components:**
    *   `ScreenGymWelcome`: Simplified fitness-focused launcher.
    *   `GymPlaylistGenerator`: Creates courses based on intensity/METs.
    *   `GymCourseWriter`: Saves custom workouts to `.crs` files.
    *   `ActorCalorieGraph`: Visualizes calorie burn over time (integrated into Gameplay).

### 5. Network Infrastructure (Simulated)
*   **Status:** Complete
*   **Key Components:**
    *   `ContentSwarmManager`: Simulates P2P package downloading/seeding.
    *   `ScreenContentNetwork`: UI for browsing and downloading content packs.
    *   `ScreenServerNode`: Idle screen simulating distributed computing jobs (AI/Rendering).
    *   `ScreenNetworkChat`: Simple chat interface accessible from Content Network.

### 6. Integration & Forks
*   **Status:** Complete
*   **Key Components:**
    *   `MsdCalculator`: Ported Etterna-style density scoring logic.
    *   `ActorMsdGraph`: Visualizes MSD density in Evaluation.
    *   `ScreenLoginQR`: StepManiaX-style QR code login simulation.
    *   `ScreenUnifiedDashboard`: "OS-like" launcher connecting all modules.
    *   **Anti-Cheat:** `ScoreSigner` using mock HMAC for score validation.

## Pending / Future Work

### 1. Real-World Integration (Post-MVP)
*   **Blockchain Bridge:** Replace `EconomyManager` simulated ledger with actual RPC calls to Ethereum/Solana/Cosmos.
*   **P2P Networking:** Replace `ContentSwarmManager` simulation with libtorrent or IPFS integration.
*   **Server Backend:** Implement a real NodeJS/Go backend for matchmaking and global chat.

### 2. Advanced Features
*   **Live Spectating:** Real-time streaming of tournament matches.
*   **Cross-Game Inventory:** API to sync assets with external games (e.g., "bob's game") for real.
*   **VR Support:** "Gym Mode" specifically optimized for VR headsets.

## Documentation
*   `docs/SYSTEM_ARCHITECTURE.md`: Technical design.
*   `docs/USER_GUIDE.md`: Player instructions.
*   `src/CMakeData-economy.cmake`: Build configuration.
