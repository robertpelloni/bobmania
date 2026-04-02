# Universal LLM Instructions

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
