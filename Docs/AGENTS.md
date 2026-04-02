# Universal LLM Agent Instructions

This file serves as the single source of truth for all AI agents working on this repository. All specific model instructions (CLAUDE, GPT, GEMINI) should reference and adhere to these core directives.

## Core Directives
1.  **Universality:** Ensure all features work across different playstyles (Pad/Keyboard).
2.  **Completeness:** Do not leave features half-implemented. Ensure UI, Backend, and Documentation are always in sync.
3.  **Documentation:** Update `CHANGELOG.md`, `VERSION`, and `DASHBOARD.md` with *every* significant change.
4.  **Verification:** Always verify code changes with `ls`, `read_file`, or compilation checks before marking tasks complete.

## Versioning Protocol
*   The project version is stored in the root `VERSION` file.
*   Format: `Major.Minor.Patch-Branch-Build` (e.g., `5.7.0-Unified-Alpha`).
*   **Update Rule:** Increment the version number in `VERSION` and `CHANGELOG.md` for every submitted pull request or significant session.

## Submodule Management
*   Submodules are located in `extern/` or `Themes/`.
*   Always check `.gitmodules` for the source of truth.
*   When updating, ensure the submodule pointer is committed.

## UI/UX Standards
*   All new screens must be defined in `Themes/default/metrics.ini`.
*   All user-facing text must be localized in `Themes/default/Languages/en.ini`.
*   Use Lua overlays in `Themes/default/BGAnimations/` for logic and display.

## Special Instructions
*   **Mining:** Do not patch `ScreenGameplay.cpp` directly for mining logic due to complexity. Use the `MiningHook.lua` overlay pattern.
*   **Economy:** Use `ECONOMYMAN` singleton for all financial transactions.
*   **Gym:** Use `GYMMAN` for all fitness data.

## Model-Specific Appendices

### Claude
*   Focus on architectural consistency and C++ safety.

### GPT
*   Focus on Lua scripting and UI generation.

### Gemini
*   Focus on documentation, reasoning, and comprehensive verification.
