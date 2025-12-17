# Comprehensive Feature List & Integration Matrix

This document catalogs the features researched and implemented for the "Unified" StepMania Network project.

## 1. Feature Reintegration Matrix

### A. Scoring & Mechanics (Etterna/ITG)
| Feature | Implementation Status | Location |
| :--- | :--- | :--- |
| **MSD (Mine-Sweeper Density) Scoring** | **Implemented (MVP)** | `src/Scoring/MsdCalculator.cpp` |
| **Hybrid Scoring (Wife)** | Planned (Roadmap) | See `UNIFICATION_ROADMAP.md` |
| **Tech/Stream Breakdown** | Partially Implemented | `GymPlaylistGenerator` uses density metrics. |

### B. User Experience (StepManiaX / DDR A)
| Feature | Implementation Status | Location |
| :--- | :--- | :--- |
| **QR Code Login** | **Implemented** | `src/Tournament/ScreenLoginQR.cpp` |
| **Touch-Friendly UI** | Foundation Laid | `ScreenGymWelcome` uses large, simple targets. |
| **Calorie/Fitness Tracking** | **Implemented** | `ScreenGymWelcome` displays goals. |

### C. Economy & Industry (New Features)
| Feature | Implementation Status | Location |
| :--- | :--- | :--- |
| **Blockchain Wallet** | **Implemented (Simulated)** | `src/Economy/EconomyManager.cpp` |
| **Betting System** | **Implemented** | `ScreenBettingEntry` & `ScreenEvaluationBetting` |
| **Music Royalties** | **Implemented** | `ScreenGameplayCommercial` |
| **DAO Governance** | **Implemented** | `ScreenGovernance.cpp` |
| **Server/Mining Mode** | **Implemented** | `ScreenServerNode.cpp` |

## 2. System Architecture Summary

The system is designed as a **Federated Network of StepMania Nodes**.

*   **Gym Nodes:** Run `ScreenGymWelcome`, utilize `GymPlaylistGenerator` for automated workouts.
*   **Arcade Nodes:** Run `ScreenTournamentLadder`, allow `ScreenBettingEntry` for competitive play.
*   **Server Nodes:** Idle machines run `ScreenServerNode` to process distributed compute jobs and earn revenue for the owner/DAO.

## 3. Future Roadmap

1.  **Backend Integration:** Replace `EconomyManager`'s local simulation with a REST API call to a real Node.js/Solidity backend.
2.  **Course Mode:** Fully enable `GymPlaylistGenerator` to populate `GAMESTATE->m_pCurCourse`.
3.  **Visual Unification:** Create a single Theme that unifies these new Screens with a consistent "Sports/Futuristic" aesthetic.
