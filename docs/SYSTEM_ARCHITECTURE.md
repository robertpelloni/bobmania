# StepMania Network: System Architecture

This document details the architectural design for the "Grand Unified" StepMania system, encompassing Athletic Leagues, Crypto Economy, Music Industry support, and Distributed Computing.

## 1. High-Level Topology

The system operates as a hybrid **Peer-to-Peer (P2P)** and **Federated** network.

*   **Nodes (Cabinets):** Each StepMania installation is a Node.
    *   **Gym Node:** Focused on fitness, limited song selection, high reliability.
    *   **Arcade Node:** Focused on earnings, full song list, betting enabled.
    *   **Home Node:** Focused on content creation and practice.
*   **The Chain (Ledger):** A decentralized ledger (simulated for MVP) tracking:
    *   User Currency (Coins)
    *   Asset Ownership (Songs, Company Shares)
    *   Match Results (Tournament Validation)

## 2. Core Modules

### A. Economy Manager (The "Wallet")
Managed by `src/EconomyManager.cpp`.
*   **Functions:**
    *   `RegisterUser()`: Generates a public/private keypair for the user.
    *   `ProcessTransaction(from, to, amount, reason)`: Moves currency.
    *   `AwardMiningReward()`: Pays the machine owner for "Server Mode" uptime.
*   **Sub-Systems:**
    *   **Music Royalty System:** Every time a song starts (`ScreenGameplay::Init`), a micro-transaction is recorded from the User -> Artist Wallet.
    *   **Betting System:** Users stake currency before a song. The system holds it in escrow. Winner takes pot minus a "House Fee" (Company Revenue).

### B. Tournament Manager (The "League")
Managed by `src/TournamentManager.cpp`.
*   **Structure:**
    *   **Ladders:** Seasonal rankings based on Elo or accumulated wins.
    *   **Brackets:** Real-time elimination trees for events.
*   **Validation:**
    *   Score packets are signed by the Client Node.
    *   Anti-Cheat: Input replay data is hashed and stored on-chain for verification.

### C. Gym & Server Mode (The "Infrastructure")
*   **Gym Interface (`ScreenGymEntry`):**
    *   Simplified login (RFID/QR).
    *   "Quick Play" (Calorie Goal or Time Goal).
    *   Auto-generated playlists.
*   **Server Mode (Background Worker):**
    *   When the machine is idle (`ScreenAttract`), it spins up a low-priority thread.
    *   This thread simulates processing "Compute Jobs" (rendering, AI training, hash solving).
    *   Earnings are credited to the Machine Owner's wallet.

## 3. Data Flow Example: A Betting Match

1.  **Challenge:** User A challenges User B in the Lobby.
2.  **Escrow:** Both users agree to bet 50 Coins. `EconomyManager` locks 100 Coins.
3.  **Gameplay:** The song is played. `Gameplay` logic runs.
4.  **Royalty:** 1 Coin is deducted from the Pot and sent to the Song Artist's address.
5.  **Result:** User A wins (Score 98% vs 95%).
6.  **Payout:**
    *   User A receives 98 Coins.
    *   1 Coin (Fee) goes to the "Company DAO" (Shareholders).
    *   (Artist already paid).
7.  **Ladder:** User A gains +15 Elo, User B loses -15 Elo.

## 4. Technical Stack (MVP Simulation)

Since we cannot deploy a real blockchain or server cluster in this session, we use **Local Simulation**:

*   **Database:** A JSON/SQLite local file `Save/Economy.db` representing the global ledger.
*   **Network:** `ScreenTournamentLadder` reads from a "Mock Cloud" class that generates fake rival data.
*   **Worker:** A simple `RageThread` that sleeps and increments a "Hashrate" counter to simulate server work.

## 4. League & Tournament Structure (The "Athletic" Layer)

To support the goal of an "Actual Athletic League", the system mimics professional sports structures (NBA, FIFA) rather than just arcade high scores.

### A. Seasonal Structure
*   **Duration:** 3 Months (Quarterly Seasons).
*   **Phases:**
    1.  **Regular Season (Weeks 1-10):** Players accumulate "League Points" (LP) through ranked matches and daily challenges.
    2.  **Playoffs (Weeks 11-12):** Top 64 players in the Pro Division enter a single-elimination bracket.
    3.  **Off-Season (2 Weeks):** Roster changes, balance patches, stat resets (Soft Reset of MMR).

### B. Divisional Hierarchy
Players are placed into buckets based on their MMR (Matchmaking Rating).
1.  **Pro League (Tier 1):** Top 1% of active players. Verified "Athletes".
2.  **Gold Division (Tier 2):** Next 9%. High-level competitive.
3.  **Silver Division (Tier 3):** Middle 40%. The bulk of the active userbase.
4.  **Bronze Division (Tier 4):** Entry level and casual play.
*   **Promotion/Relegation:** At season end, the top 15% of a division promote, bottom 15% relegate.

### C. Match Formats
*   **Ranked Duel:** 1v1, Best of 3 Songs. Winner takes LP and Betting Pot.
*   **Daily Sprint:** Asynchronous. A setlist of 4 specific songs. Highest cumulative DP (Dance Points) determines daily leaderboard position.
