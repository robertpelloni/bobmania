# Universal LLM Instructions

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

## 6. Agent-Specific Notes
See `AGENTS.md` for specific overrides.
