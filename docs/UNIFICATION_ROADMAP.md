# StepMania Feature Reintegration & Unification Roadmap

This document outlines the strategy to reintegrate fragmented features from major StepMania forks (Etterna, Project OutFox, StepManiaX) and mainline Dance Dance Revolution (DDR) into the main StepMania 5 trunk.

## 1. Feature Reintegration Matrix

### A. Scoring & Mechanics (The "Etterna" & "ITG" Pillar)
| Feature | Source | Description | Reintegration Strategy |
| :--- | :--- | :--- | :--- |
| **MSD (Mine-Sweeper Density) Scoring** | Etterna | Difficulty calculation based on chord density and technical complexity rather than simple step count. | Port the `Calculator` class from Etterna. Expose MSD values to Lua for theme display. |
| **Wife/JudgeTiming** | Etterna | A more precise timing window system (millisecond based) replacing standard "Perfect/Great" frames. | Implement `TimingWindowSeconds` preferences and a new `Grade_Tier01` logic. |
| **FA+ / Pro Timing** | ITG / ECFA | Tighter timing windows for competitive play (Fantastics). | Add standard presets in `Preferences.ini` for "Competitive Mode". |
| **Groove Radar 2.0** | DDR (Modern) | Updated radar including "Chaos" and "Freeze" metrics. | Update `GrooveRadar.cpp` to calculate modern stream/voltage metrics. |

### B. Engine & Performance (The "OutFox" Pillar)
| Feature | Source | Description | Reintegration Strategy |
| :--- | :--- | :--- | :--- |
| **New Renderer (Bgfx/Vulkan)** | OutFox | Modern graphics API support for better performance on new hardware. | *Long-term:* Replace `RageDisplay` with a modular backend. *Short-term:* Optimize `RageDisplay_OGL`. |
| **Universal Game Modes** | OutFox | Support for Pump, Techno, ez2, etc., in a single binary. | Ensure `NoteSkinManager` and `GameManager` are decoupled from "Dance" specific logic. |
| **FFmpeg 5+ Support** | OutFox | Better video decoding for background videos. | Update `RageMovieTexture` to link against newer FFmpeg libraries. |

### C. User Experience & Hardware (The "StepManiaX" & "DDR" Pillar)
| Feature | Source | Description | Reintegration Strategy |
| :--- | :--- | :--- | :--- |
| **Touchscreen UI** | StepManiaX | Big, swipe-friendly buttons for public/arcade use. | Create a `Theme_Touch` derived theme with larger hitboxes and drag-scroll support. |
| **QR Code Login** | DDR A / SMX | Login via mobile app scan. | Implement `ScreenLoginQR` handling string input from a scanner (simulated as keyboard). |
| **Party / Battle Mode** | DDR | "Tug of War" lifebars and attack modifiers. | Enhanced `ScoreDisplayBattle` and `Attack` classes (already partially in SM5, needs polish). |
| **Calories & Fitness Tracking** | DDR / SMX | Accurate calorie count based on weight + step energy. | Enhance `Profile` to store weight history and export to `HealthManager`. |

## 2. Unification Strategy

The goal is to create a "Super-Client" that defaults to standard SM5 behavior but allows enabling these "Pillars" via Modules.

1.  **Modular Scoring Engine:** Allow the user to select `ScoringEngine` in preferences (Standard, Etterna_Msd, OutFox_Hybrid).
2.  **Unified Theme API:** Standardize Lua hooks so themes from OutFox or Etterna can run with minimal modification.
3.  **Hardware Abstraction:** A new input layer that natively supports newer pads (SMX Stages, L-Tek) without complex mapping.

## 3. Immediate Implementation Targets (MVP)

For this session, we focus on the **Competitive & Economy** layer:
1.  **Tournament Ladder UI:** A unification of the "Leaderboard" concept found in all forks.
2.  **Betting System:** A novel feature not present in any fork, bridging the gap between "Game" and "Sport".
