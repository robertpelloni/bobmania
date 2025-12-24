# Handoff Document: StepMania 5.2 (5.1-new) Merge Project & Token Research

## 1. StepMania 5.2 Merge Status
We have successfully merged key features from the **ITGMania** fork and achieved significant **NotITG** parity in the **StepMania 5.1-new** branch.

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

4.  **NotITG Parity: Shader Support (Advanced)**
    *   **Shader Loading:** Implemented `LoadShaderFromFile`, `SetShader`, `GetShader`, `DeleteShader` in `RageDisplay` / `RageDisplay_OGL`.
    *   **Per-Actor Shaders:** Added `Actor::SetShader(path)` and `Actor::SetShader(vert, frag)` Lua bindings. Hooks `BeginDraw`/`EndDraw` to safely apply and restore shaders, supporting nested ActorFrames.
    *   **Uniform Passing:** Added `Actor::SetUniform(name, val...)` Lua binding. Supports floats, vec2, vec3, vec4. Automatically applies uniforms when the shader is active during draw.
    *   **Vertex Shaders:** Explicit support via `SetShader(vert, frag)`.
    *   **Files:** `src/RageDisplay.h`, `src/RageDisplay_OGL.h`, `src/RageDisplay_OGL.cpp`, `src/Actor.h`, `src/Actor.cpp`.

5.  **NotITG Parity: Render Targets**
    *   **Status:** **Supported** via existing `ActorFrameTexture` class.
    *   **Lua Binding:** `ActorFrameTexture` exposes `Create`, `SetTextureName`, `EnableDepthBuffer`, etc.
    *   **Usage:** Create AFT, add children, capture to texture, use texture on Sprite.

6.  **Mine Fix (DinsFire64)**
    *   **Implementation:** Fixed logic in `Player::Step` to correctly trigger mines on fresh presses when `REQUIRE_STEP_ON_MINES` is false.
    *   **Details:** Changed condition from `( REQUIRE_STEP_ON_MINES == !bHeld )` to `( !REQUIRE_STEP_ON_MINES || !bHeld )`.
    *   **Files:** `src/Player.cpp`.

7.  **Gameplay Modes: BothAtOnce**
    *   **Implementation:** Added `BothAtOnce` preference.
    *   **Details:** Mirrors inputs from any player to all enabled players in `ScreenGameplay`. Useful for "2 player 1 controller" modfiles.
    *   **Files:** `src/PrefsManager.h`, `src/PrefsManager.cpp`, `src/ScreenGameplay.cpp`.

8.  **NotITG Parity: NotePath Actor**
    *   **Implementation:** Added new `NotePath` actor class.
    *   **Details:** Allows visualizing the future path of arrows for a specific column. Supports configurable resolution and draw range (beats).
    *   **Lua Bindings:** `SetPlayer(pn)`, `SetColumn(col)`, `SetDrawRange(start, end)`, `SetResolution(steps)`.
    *   **Files:** `src/NotePath.h`, `src/NotePath.cpp`.

9.  **ITGMania Parity: Visual Delay & Held Misses**
    *   **Visual Delay:** Added `VisualDelaySeconds` player option. Adjusts the visual beat/time of arrows relative to the music without affecting scoring offset.
    *   **Held Misses:** Added `ScoreMissedHoldsAndRolls` player option. Forces missed holds to count as "LetGo" (Held Miss) judgments even if the theme metric is disabled.
    *   **Files:** `src/PlayerOptions.h`, `src/PlayerOptions.cpp`, `src/PlayerState.h`, `src/PlayerState.cpp`, `src/Player.cpp`.

## 2. Economy & Token Foundation
We have laid the groundwork for a "Tip Economy" by implementing the **EconomyManager**.

### EconomyManager
*   **Implementation:** Singleton `ECONOMYMAN` initialized in `StepMania.cpp`.
*   **Functionality:** Simulates a wallet balance and tipping functionality.
*   **Lua Bindings:**
    *   `EconomyManager:GetBalance()`: Returns current mock balance.
    *   `EconomyManager:GetWalletAddress()`: Returns mock address.
    *   `EconomyManager:SendTip(address, amount)`: Deducts balance and logs the transaction.
    *   `EconomyManager:IsConnected()`: Checks status.
*   **Files:** `src/Economy/EconomyManager.h`, `src/Economy/EconomyManager.cpp`, `src/CMakeData-singletons.cmake`, `src/StepMania.cpp`.

## 3. Token Foundation Research: Tempo vs. Others
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
