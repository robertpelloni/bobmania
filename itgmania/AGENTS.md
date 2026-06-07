# Agent Instructions
This file acts as a router to the Universal LLM Instructions.

**Read First:** `docs/LLM_UNIVERSAL.md`

## Specific Notes
*   **Context:** This project is a massive fork/overhaul of StepMania 5.
*   **Behavior:** Be autonomous. Create files, fix errors, and document as you go.
*   **Tools:** Use `grep`, `find`, and `read_file` extensively before asking questions.
# Agent Instructions for StepMania 5.2 Unified

This repository is a unification of StepMania 5.1-new, ITGMania, NotITG, and Etterna.

## Core Directives
1.  **Backwards Compatibility:**
    *   **Strict Adherence:** Changes MUST NOT break existing StepMania 5.0/5.1 themes or simfiles.
    *   **Lua API:** Do not rename or remove existing API calls. Use aliases if needed (`Compatibility.cpp`).
    *   **Engine:** Do not alter the core timing window or scoring logic without an option toggle.

2.  **Versioning:**
    *   On every major change or release build, increment the version in:
        *   `VERSION` (Root)
        *   `src/ProductInfo.h`
        *   `src/ProductInfo.inc`
        *   `configure.ac`
    *   Format: `5.2.x-Unified`

3.  **Changelog:**
    *   Log all changes in `Docs/Changelog_Unified.txt`.
    *   Format: `[Date] [Component] Description`.

4.  **Submodules:**
    *   Dependencies are located in `extern/`.
    *   Do not update submodules unless necessary for security or critical bug fixes. Compatibility is priority.

5.  **New Features:**
    *   Follow the patterns established in `src/` (e.g., `DiscordManager`, `GrooveStatsManager`).
    *   Expose functionality to Lua whenever possible.

6.  **Code Style:**
    *   Follow existing C++ conventions found in `src/`.
    *   Use spaces, not tabs.
    *   Comment complex logic.
