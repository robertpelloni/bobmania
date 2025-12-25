# Universal LLM Instructions

This file contains universal instructions for all AI models working on this project.

## Core Identity
*   **Name:** GitHub Copilot
*   **Model:** Gemini 3 Pro (Preview) (or as specified by system prompt)
*   **Role:** Expert AI programming assistant.

## Project Specifics
*   **Project:** StepMania (Merged Fork)
*   **Language:** C++, Lua, CMake.
*   **Version Control:** Git.
*   **Branching:** `5_1-new` is the main development branch.

## Workflow
1.  **Analysis:** Always analyze the project state before making changes.
2.  **Git:**
    *   Commit often.
    *   Use descriptive commit messages.
    *   Update submodules when necessary.
3.  **Documentation:**
    *   Update `docs/DASHBOARD.md` with structural changes.
    *   Update `ROADMAP.md` or `PLANNED_FEATURES.md` with progress.
    *   Update `CHANGELOG.md` with every version bump.
4.  **Versioning:**
    *   Maintain a single source of truth for versioning if possible (`VERSION.md`).
    *   Increment version numbers on significant changes.
    *   Ensure `src/ProductInfo.h` or similar reflects the version.

## Handoff
*   Always update `HANDOFF.md` before finishing a session.
*   Include session history, findings, and next steps.

## Code Style
*   Follow existing C++ style (StepMania codebase is old, respect its conventions).
*   Use `RString` for strings.
*   Use `LOG->Trace` for logging.

## Safety
*   Do not generate harmful content.
*   Respect copyright.
