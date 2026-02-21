# Project Dashboard

## Directory Structure
*   **`src/`**: Core Engine Code (C++)
    *   `Economy/`: EconomyManager, Marketplace
    *   `Gym/`: GymManager, CalorieGraph
    *   `Tournament/`: TournamentManager, Brackets
    *   `Unified/`: Dashboard, MissionManager
    *   `Network/`: GameClient, Spectator
*   **`Themes/`**: Lua-based Skins
    *   `default/`: Primary unified theme
    *   `Simply-Love-SM5/`: ITG-faithful submodule
*   **`extern/`**: External Dependencies (Submodules)
    *   `bobcoin/`: Cryptocurrency mock (Privacy-focused)
*   **`Docs/`**: Documentation

## Submodules
| Name | Path | Version/Tag | Description |
| :--- | :--- | :--- | :--- |
| **Bobcoin** | `extern/bobcoin` | *HEAD* | Mock implementation of the project's economy token. |
| **Simply Love** | `Themes/Simply-Love-SM5` | *HEAD* | The standard theme for ITG/Stamina play. |

## Build Status
*   **Version:** 5.7.0-Unified-Alpha
*   **Build System:** CMake
*   **Primary Branch:** `unified-backend-implementation`

## Feature Status
*   [x] **Economy:** Active
*   [x] **Gym:** Active
*   [x] **Tournament:** Active
*   [ ] **Missions:** Pending (Backend)
*   [ ] **NetPlay:** Stubbed
