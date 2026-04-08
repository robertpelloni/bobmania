# Universal LLM Instructions

<<<<<<< HEAD:docs/LLM_UNIVERSAL.md
**Version:** 1.0 (2025-12-27)
**Scope:** Applies to all AI Agents (Claude, Gemini, GPT, Copilot) working on this repository.

## 1. Core Directives
*   **Safety First:** Do not implement features that could compromise user security (e.g., real private key handling without encryption).
*   **MVP Mindset:** Prioritize functional simulations (MVPs) over perfect real-world integrations initially.
*   **Preserve Legacy:** Respect the existing StepMania 5 architecture. Do not refactor core engine files (`ScreenGameplay`, `Actor`) unless necessary. Use hooks/stubs where possible.
*   **Documentation:** Always update `HANDOFF.md`, `CHANGELOG.md`, and `docs/ROADMAP.md` before finishing a session.

## 2. Coding Style (C++)
*   **Standard:** C++98/03 (Legacy StepMania standard) mostly, but C++11 is acceptable for new modules.
*   **Includes:** Use relative paths from `src/` (e.g., `#include "Economy/EconomyManager.h"`).
*   **Logging:** Use `LOG->Trace()`, `LOG->Warn()`, `LOG->Info()`. Do not use `std::cout`.
*   **Singletons:** Access global managers via their `Instance()` method or global pointer macros (`PREFSMAN`, `SCREENMAN`).

## 3. Versioning Protocol
*   **Single Source of Truth:** The file `VERSION` in the root directory contains the current version string.
*   **Bumping:** When completing a feature, increment the version in `VERSION`, `src/ProductInfo.inc`, `src/CMakeLists.txt` (if applicable), and `CHANGELOG.md`.
*   **Tagging:** Feature branches should be named `feature/name`. Releases should be tagged `vX.Y.Z`.

## 4. Project Structure
*   `src/Economy/`: Cryptocurrency, Banking, Governance.
*   `src/Tournament/`: Ladders, Brackets, matchmaking.
*   `src/Gym/`: Fitness modes, calorie tracking.
*   `src/Network/`: P2P and Server Node logic.
*   `src/arch/`: Hardware abstraction (Input, Sound, Loading, VR).

## 5. Build Instructions
*   **Local Test:** `g++ -fsyntax-only -I src path/to/file.cpp`
*   **Full Build:** `cmake -G "Unix Makefiles" .. && make` (Requires dependencies: X11, ALSA, libmad, etc.)

## 6. Submodule Management
*   **Monorepo Strategy:** Treat submodules (e.g., `extern/bobcoin`) as active development zones.
*   **Workflow:**
    1.  Edit submodule code directly.
    2.  Commit and push to the submodule's remote.
    3.  Bump the version number in the submodule.
    4.  Update the parent repo's pointer.
*   **Bobcoin:** This is the primary economy token. Ensure it is fast, anonymous, and integrated.

## 7. Agent-Specific Notes
See `AGENTS.md` for specific overrides.
=======
This document serves as the master guide for all AI Developer models (Jules, Claude, Gemini, GPT) working on the Unified StepMania project. It supersedes any individual agent instruction files.

## Project Context
You are working on the `5.1-new` branch of a massive StepMania fork. The goal is "Unified StepMania," a platform that merges the fitness capabilities of Gym Mode, the economy of Bobcoin, the P2P networking of Content Swarm, and the competitive features of Etterna/NotITG.

## Core Directives

1.  **Preserve Legacy:** We rigidly enforce Lua 5.1 compatibility. Do not attempt to upgrade the engine to Lua 5.3+ (as seen in Project OutFox) or use advanced Lua operators. Legacy themes must remain functional.
2.  **The "Manager" Pattern:** All new features must be encapsulated in global Singleton Managers (e.g., `ECONOMYMAN`, `GYMMAN`) located in `src/` subdirectories. They must be registered in `src/global.h`, instantiated in `src/StepMania.cpp`, updated in `src/GameLoop.cpp`, bound via `Luna<T>`, and added to `src/CMakeData-singletons.cmake`.
3.  **UI Separation:** The C++ backend handles logic. The Lua frontend (`Themes/default/`) handles 100% of the UI. Never hardcode UI elements in C++.
4.  **Exhaustive Completeness:** Every C++ feature *must* be fully represented in the UI, fully documented in `Docs/Manual_Unified.md`, and completely hooked up via the global `HelpOverlay`. No half-measures.
5.  **Mocking:** If an external dependency (like a real blockchain or a Bluetooth heart rate monitor) is missing, you must build a robust, structurally sound "Mock" or "Stub" in C++ that simulates the exact behavior. The UI must not know it is a mock.

## Versioning & Changelogs
There is only one source of truth for the project version: the file `VERSION` located at the root of the repository.
*   **When making a change:** You must manually increment the version string in `VERSION` (e.g., from `5.7.2-Unified-Beta` to `5.7.3-Unified-Beta`).
*   **Documentation:** You must log your changes in `CHANGELOG.md`, referencing the new version number.
*   **Commits:** Your git commit message MUST include the version bump (e.g., `chore: Add feature XYZ (v5.7.3)`).

## File IO & System Calls
Always use StepMania's `RageFile` wrapper for file operations to ensure cross-platform compatibility and correct virtual filesystem (VFS) pathing (`Save/`, `Data/`). Never use `std::fstream`.

## Commenting Code
Comment code aggressively but intelligently. Explain *why* a decision was made, what the side effects are, and how it interacts with the Lua layer. If a mock is used, comment exactly how it should be replaced with real API calls.
>>>>>>> origin/unified-ui-features-13937230807013224518:Docs/LLM_UNIVERSAL.md
