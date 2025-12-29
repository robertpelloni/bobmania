# StepMania Network: Project Roadmap

## 1. Project Goal
Transform StepMania 5 into a connected platform combining rhythm gaming, cryptocurrency economics, competitive tournaments, and fitness tracking.

## 2. Release Status: v5.4.2-Maintenance

### Completed Features (Integrated)
*   **Economy & Marketplace:**
    *   Simulated Ledger & Wallets (`EconomyManager`).
    *   Asset Marketplace (`ScreenMarketplace`).
    *   DAO Governance (`ScreenGovernance`).
    *   Cross-Game Inventory Sync (`AssetSyncManager`).
*   **Competition:**
    *   Elo Ladders & Divisions (`ScreenTournamentLadder`).
    *   Betting System (`ScreenBettingEntry`).
    *   Live Spectating / Broadcasting (`SpectatorManager`, `StreamManager`).
*   **Fitness:**
    *   Gym Mode & Playlist Generator (`ScreenGymWelcome`).
    *   Calorie Burn Visualization (`ActorCalorieGraph`).
*   **Connectivity:**
    *   Unified Dashboard (`ScreenUnifiedDashboard`).
    *   Network Chat & Client (`GameClient`, `ScreenNetworkChat`).
    *   P2P Swarm Simulation (`ContentSwarmManager`).
    *   **Backend Server:** Node.js WebSocket server (`server/index.js`) handling Matchmaking queues and Chat relay.
*   **VR Support:**
    *   Architecture Hooks (`ArchHooks_VR`).
    *   Render Loop & Matrix Stubs.

### Pending / Future Work (Post-v5.6.0)
*   **Bobcoin Integration:** Fully implement `BobcoinBridge` to communicate with the `bobcoin` submodule.
*   **VR Implementation:** Connect `ArchHooks_VR` to OpenXR or SteamVR SDKs.
*   **Live Streaming:** Implement real video encoding logic in `StreamManager::PushFrame` (currently stubbed).

## 3. Architecture
See `docs/DASHBOARD.md` for dependency versions and directory layout.
