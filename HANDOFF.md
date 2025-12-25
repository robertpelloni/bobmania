# Handoff Document: StepMania 5.2 (5.1-new) Merge Project & Token Research

## 1. StepMania 5.2 Merge Status
We have successfully merged key features from the **ITGMania** fork and achieved significant **NotITG** parity in the **StepMania 5.1-new** branch.

### Completed Features
1.  **Load New Songs (SSMReloadSongs)**
    *   **Implementation:** Added SongManager::LoadNewSongs() and exposed it to Lua.
    *   **Details:** Scans Songs/ and AdditionalSongs/ for new folders and loads them. Skips existing loaded songs to ensure performance.
    *   **Files:** src/SongManager.h, src/SongManager.cpp.

2.  **Fast Profile Switching (SSMProfileSwitch)**
    *   **Implementation:** Added ProfileManager::NextLocalProfile(PlayerNumber pn) and exposed it to Lua.
    *   **Details:** Cycles through local profile IDs for the given player, including an empty state (Unjoined/Guest). Reloads the profile immediately.
    *   **Files:** src/ProfileManager.h, src/ProfileManager.cpp.

3.  **Rate Mod Options (Pitch Dependent vs Independent)**
    *   **Implementation:** Added PitchDependentRate preference and logic in ScreenGameplay.
    *   **Details:**
        *   Default (False): Pitch Independent (Time Stretch, uses RageSoundReader_SpeedChange implicitly via RageSound default).
        *   Enabled (True): Pitch Dependent (Vinyl/Chipmunk). Sets RageSound Pitch property equal to Speed property.
    *   **Files:** src/PrefsManager.h, src/PrefsManager.cpp, src/ScreenGameplay.cpp.

4.  **NotITG Parity: Shader Support (Advanced)**
    *   **Shader Loading:** Implemented LoadShaderFromFile, SetShader, GetShader, DeleteShader in RageDisplay / RageDisplay_OGL.
    *   **Per-Actor Shaders:** Added Actor::SetShader(path) and Actor::SetShader(vert, frag) Lua bindings. Hooks BeginDraw/EndDraw to safely apply and restore shaders, supporting nested ActorFrames.
    *   **Uniform Passing:** Added Actor::SetUniform(name, val...) Lua binding. Supports floats, vec2, vec3, vec4. Automatically applies uniforms when the shader is active during draw.
    *   **Vertex Shaders:** Explicit support via SetShader(vert, frag).
    *   **Files:** src/RageDisplay.h, src/RageDisplay_OGL.h, src/RageDisplay_OGL.cpp, src/Actor.h, src/Actor.cpp.

5.  **NotITG Parity: Render Targets**
    *   **Status:** **Supported** via existing ActorFrameTexture class.
    *   **Lua Binding:** ActorFrameTexture exposes Create, SetTextureName, EnableDepthBuffer, etc.
    *   **Usage:** Create AFT, add children, capture to texture, use texture on Sprite.

6.  **Mine Fix (DinsFire64)**
    *   **Implementation:** Fixed logic in Player::Step to correctly trigger mines on fresh presses when REQUIRE_STEP_ON_MINES is false.
    *   **Details:** Changed condition from ( REQUIRE_STEP_ON_MINES == !bHeld ) to ( !REQUIRE_STEP_ON_MINES || !bHeld ).
    *   **Files:** src/Player.cpp.

7.  **Economy & Tournament System (Merged)**
    *   **Economy:** Implemented EconomyManager for handling currency and transactions.
    *   **Tournament:** Implemented ScreenLoginQR for tournament login and bracket management.
    *   **Unified Dashboard:** Added ScreenUnifiedDashboard for player stats.
    *   **Gym Mode:** Added ScreenGymWelcome for training.
    *   **Files:** src/Economy/*, src/Tournament/*, src/Unified/*, src/Gym/*.

## 2. Token Foundation Research: Tempo vs. Others
We analyzed **Tempo (tempoxyz/tempo)** as a candidate for a high-volume, low-fee tip economy coin foundation.

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
| **Fees** | <.001 (Stable) | **Zero** | ~.00025 (Volatile) |
| **Speed** | Sub-second | Instant | ~0.4s |
| **Smart Contracts** | **Yes (EVM)** | No | Yes (Rust) |
| **Best For** | Programmable Tips, Stablecoins | Pure P2P Value Transfer | High Perf DApps |

### Recommendation
*   **Use Tempo** if you need smart contracts (e.g., conditional tips, DAO governance) and want a user-friendly gasless experience.
*   **Use Nano** if you want the absolute simplest, feeless P2P transfer with no complex logic.

## 3. Session History (Dec 25, 2025)
*   **Merged Feature Branches:** economy-tournament-mvp and itgmania-features into 5_1-new.
*   **Resolved Conflicts:** Fixed conflicts in ProfileManager.cpp, ScreenGameplay.cpp, and others.
*   **Documentation:** Created docs/DASHBOARD.md, LLM_INSTRUCTIONS.md, VERSION.md, CHANGELOG.md.
*   **LLM Instructions:** Updated CLAUDE.md, GEMINI.md, GPT.md, copilot-instructions.md, AGENTS.md.
*   **Version Bump:** Updated version to 5.1.0.
