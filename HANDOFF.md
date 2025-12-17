# Handoff Document: StepMania 5.2 (5.1-new) Merge Project & Token Research

## 1. StepMania 5.2 Merge Status
We have successfully merged key features from the **ITGMania** fork into the **StepMania 5.1-new** branch.

### Completed Features
1.  **Load New Songs (`SSMReloadSongs`)**
    *   **Implementation:** Added `SongManager::LoadNewSongs()` and exposed it to Lua.
    *   **Details:** Scans `Songs/` and `AdditionalSongs/` for new folders and loads them. Skips existing loaded songs to ensure performance.
    *   **Files:** `src/SongManager.h`, `src/SongManager.cpp`.

2.  **Fast Profile Switching (`SSMProfileSwitch`)**
    *   **Implementation:** Added `ProfileManager::NextLocalProfile(PlayerNumber pn)` and exposed it to Lua.
    *   **Details:** Cycles through local profile IDs for the given player, including an "empty" state (Unjoined/Guest). Reloads the profile immediately.
    *   **Files:** `src/ProfileManager.h`, `src/ProfileManager.cpp`.

3.  **Rate Mod Options (Pitch Dependent vs Independent)**
    *   **Implementation:** Added `PitchDependentRate` preference and logic in `ScreenGameplay`.
    *   **Details:**
        *   Default (False): Pitch Independent (Time Stretch, uses `RageSoundReader_SpeedChange` implicitly via `RageSound` default).
        *   Enabled (True): Pitch Dependent (Vinyl/Chipmunk). Sets `RageSound` Pitch property equal to Speed property.
    *   **Files:** `src/PrefsManager.h`, `src/PrefsManager.cpp`, `src/ScreenGameplay.cpp`.

### Pending / Future Work
*   **Mine Fix (DinsFire64):** Not implemented. Logic requires deeper investigation into `Player.cpp` interactions with hold releases and overlapping taps.
*   **NotITG Parity:** Lua modding API (Actors, Shaders) is a large task remaining.

## 2. Token Foundation Research: Tempo vs. Others
We analyzed **Tempo (`tempoxyz/tempo`)** as a candidate for a high-volume, low-fee "tip economy" coin foundation.

### Tempo Analysis
*   **Focus:** Stablecoin payments at scale.
*   **Key Pros:**
    *   **Stablecoin Gas:** Users pay fees in USD stablecoins (great UX).
    *   **Dedicated Payment Lanes:** Prevents network congestion from other apps affecting payments.
    *   **Fee Sponsorship:** Apps can pay gas for users (ideal for tip onboarding).
    *   **EVM Compatible:** Easy for devs to build on.
*   **Cons:** Newer ecosystem compared to Solana/Nano.

### Comparison Table
| Feature | Tempo | Nano (XNO) | Solana (SOL) |
| :--- | :--- | :--- | :--- |
| **Fees** | <$0.001 (Stable) | **Zero** | ~$0.00025 (Volatile) |
| **Speed** | Sub-second | Instant | ~0.4s |
| **Smart Contracts** | **Yes (EVM)** | No | Yes (Rust) |
| **Best For** | Programmable Tips, Stablecoins | Pure P2P Value Transfer | High Perf DApps |

### Recommendation
*   **Use Tempo** if you need smart contracts (e.g., conditional tips, DAO governance) and want a user-friendly "gasless" experience.
*   **Use Nano** if you want the absolute simplest, feeless P2P transfer with no complex logic.
