# StepMania Network: System Architecture

## 1. Core Modules

### 1.1 Economy Layer (`src/Economy/`)
The economic backbone is managed by the `EconomyManager` singleton.
*   **Ledger:** A persistent map of `WalletAddress` to `CurrencyAmount`, saved to `Save/Economy.ini`.
*   **Asset System:** Supports specialized assets (Titles, Licenses, Shares) with unique properties.
*   **Betting:** Features a secure state machine for match wagering (`StartMatchBet` -> `ResolveMatchBet`), holding funds in an escrow wallet.
*   **Governance:** Provides `GetShareCount()` and `CalculateDividend()` to support DAO operations.

### 1.2 Tournament Layer (`src/Tournament/`)
*   **Ranking:** Implements an Elo rating system.
*   **Structure:** `ScreenTournamentLadder` visualizes the player's position in Divisions (Pro/Gold/Silver).
*   **Visualization:** `TournamentBracket` generates visual trees for match history.

### 1.3 Gym & Fitness (`src/Gym/`)
*   **Playlist Gen:** `GymPlaylistGenerator` algorithmically creates courses based on intensity.
*   **Tracking:** `ActorCalorieGraph` renders real-time calorie burn data during gameplay.
*   **Data:** `GymCourseWriter` exports sessions to standard `.crs` format.

### 1.4 Network Layer (`src/Network/`)
*   **P2P Simulation:** `ContentSwarmManager` mimics BitTorrent behavior for content distribution.
*   **Chat:** `ScreenNetworkChat` provides a lobby interface.
*   **Distributed Computing:** `ScreenServerNode` gamifies idle time by "mining" currency through simulated tasks.

## 2. Integration Points

### 2.1 Unified Dashboard
`ScreenUnifiedDashboard` acts as the operating system for the network features, providing a central hub to access the Economy, Gym, and Tournament screens.

### 2.2 Engine Hooks
*   **Gameplay:** `ScreenGameplay.cpp` hooks into `Init` to display the `ActorCalorieGraph` in Gym modes.
*   **Evaluation:** `ScreenEvaluation.cpp` hooks into the result flow to trigger `EconomyManager::ResolveMatchBet` and display `ActorMsdGraph`.
*   **Profile:** `ProfileManager` extended with `LoadProfileFromID` to support QR-based login flow.

### 2.3 External Dependencies
The project relies on a set of vendored libraries located in `extern/` to ensure cross-platform compatibility and stability:
*   **Scripting:** Lua 5.1
*   **Media:** ffmpeg, libjpeg, libpng, mad, vorbis/ogg
*   **Parsing:** jsoncpp, pcre
*   **System:** glew, zlib

## 3. Data Flow
1.  **Login:** User logs in (Local or QR).
2.  **Dashboard:** User selects "Wager Battle".
3.  **Betting:** `ScreenBettingEntry` deducts funds to Escrow.
4.  **Gameplay:** User plays song. `ActorCalorieGraph` tracks effort.
5.  **Evaluation:**
    *   Score is signed by `ScoreSigner` (using `libtomcrypt` logic).
    *   `EconomyManager` checks score against difficulty.
    *   Funds released from Escrow to Winner (Player or House).
    *   Elo updated.
6.  **Save:** All state persisted to `Economy.ini` and `Stats.xml`.
