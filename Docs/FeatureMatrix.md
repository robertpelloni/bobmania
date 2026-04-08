# Unified StepMania Feature Matrix

This document maps features from various rhythm game franchises to their implementation in this Unified fork.

## DDR & Arcade Features
| Feature | Source Game | Unified Implementation |
| :--- | :--- | :--- |
| **Mission Mode** | DDR Extreme / MAX | `ScreenMissionSelect` (Dashboard -> Missions) |
| **Course Mode** | DDR / ITG | Native `ScreenSelectCourse` (Arcade Mode) |
| **Workout / Calories** | DDR / Wii | `ScreenGymWelcome` (Dashboard -> Gym) |
| **Battle Mode** | DDR X | `ScreenTournamentLadder` (Concept) |
| **Shop / Unlocks** | DDR A / ITG | `ScreenMarketplace` (Dashboard -> Economy) |
| **E-Amusement / Net** | DDR / PiU | `ScreenNetRoom` & `ScreenWalletHistory` |

## ITG & Modifiers
| Feature | Source Game | Unified Implementation |
| :--- | :--- | :--- |
| **Detailed Stats** | ITG / OpenITG | `ScreenEvaluation` (Enhanced Metrics) |
| **Custom Songs** | ITG | Fully Supported via `Songs/` |
| **Mod Menu** | ITG | `ScreenPlayerOptions` (includes ProTiming, ScreenFilter) |
| **Stamina Charts** | ITG Community | `ScreenGymWorkout` (Playlist generation) |

## Etterna & Competitive
| Feature | Source Game | Unified Implementation |
| :--- | :--- | :--- |
| **Leaderboards** | Etterna | `ScreenTournamentLadder` |
| **Judge Scaling** | Etterna | `PlayerOptions` (ProTiming) |
| **Replay System** | Etterna | *Planned* |
| **Wife Scoring** | Etterna | *Planned* (uses standard DP/Migs currently) |

## Pump It Up (PIU)
| Feature | Source Game | Unified Implementation |
| :--- | :--- | :--- |
| **Quest World** | PIU Zero | `ScreenMissionSelect` |
| **Co-Op Play** | PIU | Supported via `Style` (Routine/Couple) |
| **Rush Mode** | PIU | *Planned* (Rate Mods in Options) |

## OutFox & Modern
| Feature | Source Game | Unified Implementation |
| :--- | :--- | :--- |
| **Lua Modding** | OutFox | Full Lua 5.1 Sandbox + `Actor` hooks |
| **Tiny-C Support** | OutFox | *Not Implemented* (Security/Portability focus) |
| **Online Play** | OutFox | `ScreenNetRoom` |

## Economy (Unique)
| Feature | Source Game | Unified Implementation |
| :--- | :--- | :--- |
| **Bobcoin Mining** | Custom | `05 Economy.lua` / `ScreenMarketplace` |
| **Betting** | Custom | `ScreenBettingEntry` |
| **Assets** | Custom | `ScreenWalletHistory` |
