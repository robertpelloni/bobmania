# Unified StepMania Vision & Comprehensive Design Document

## The Ultimate Goal
To create the definitive, universal rhythm game platform that unifies the fragmented StepMania ecosystem. "Unified StepMania" aims to integrate the best features from all major forks (ITGMania, Etterna, OutFox, NotITG) and official franchises (DDR, ITG, PIU) into a single, cohesive, community-driven experience.

## Core Pillars & Detailed Specifications

### 1. Universality & Engine Parity
A single engine that supports all playstyles:
*   **Arcade / Stamina:** Perfect timing windows, vast song library support.
*   **Tech / Modding:** Full NotITG compatibility (Lua hooks, ActorProxies, Shader Mods).
*   **Keyboard Competitive:** Etterna-style Wife scoring (J4), MSD calculation, and density graphs.
*   **Pump & Multi-Panel:** Support for 5-panel, 6-panel, and 9-panel modes.

### 2. Economy & Progression (Play-to-Earn)
A persistent layer of engagement featuring a virtual economy powered by **Bobcoin**.
*   **Wallet System:** Full transaction history, sending/receiving funds.
*   **Marketplace:** Buy/Sell songs, themes, and avatars using in-game currency.
*   **Mining:** Passive income generation through gameplay performance ("Proof of Dance").
*   **Asset Management:** Inventory system for managing owned digital goods.

### 3. Fitness & Health (Gym Mode)
A dedicated career mode for physical improvement.
*   **Workout Plans:** Structured sets targeting specific muscle groups or cardio zones.
*   **Profile Tracking:** Weight, BMI, and calorie burn history over time.
*   **Integration:** Sync with external health APIs (planned).

### 4. Competitive Esports (Tournament Mode)
Automated systems for organized play.
*   **Ladders:** ELO-based ranking systems for 1v1 and team play.
*   **Matchmaking:** Queue system to find opponents of similar skill.
*   **Spectator TV:** Live broadcasting of high-level matches with commentary tools.

### 5. Missions & Quests
Dynamic objectives to guide player progression.
*   **Daily/Weekly Quests:** Rotational challenges for rewards.
*   **Campaign Mode:** A structured path through song packs with unlocking mechanics.

### 6. User Experience (UI/UX)
**Mandate:** "Every feature must be fully represented in UI with comprehensive details."
*   **Tooltips & Help:** Every button and concept must have explanatory text.
*   **Consistency:** Unified design language across all new screens (Marketplace, Gym, etc.).
*   **Accessibility:** Clear labels, high-contrast text, and intuitive navigation.

## Feature Roadmap & Status

### Phase 1: Foundation (Completed)
*   [x] **Unified Dashboard:** Central hub connecting all modes.
*   [x] **Economy Backend:** `EconomyManager` (Inventory, Mining, Ledger).
*   [x] **Gym Backend:** `GymManager` (Workouts, Profile).
*   [x] **Tournament Backend:** `TournamentManager` (Ladders, Matches).

### Phase 2: Gameplay Depth (In Progress)
*   [ ] **Wife Scoring:** Advanced scoring metric (J4 implementation).
*   [ ] **Replay System:** Recording and playback of inputs.
*   [ ] **Mission Mode:** Full UI implementation for quest selection.

### Phase 3: Network & Social (Planned)
*   [ ] **NetPlay:** Robust lobby system.
*   [ ] **Social Hub:** Chat, friends list, and guild management.

## Architectural Principles
*   **Monorepo:** All dependencies (ffmpeg, bobcoin, lua) managed in `extern/`.
*   **Lua-First:** Empower creators by exposing deep engine hooks to Lua.
*   **Safety:** Maintain a secure sandbox while allowing creative freedom.

## Conclusion
Unified StepMania is not just a simulator; it is a platform for the next generation of rhythm gamers, combining physical fitness, competitive esports, and creative expression into a single, polished product.
