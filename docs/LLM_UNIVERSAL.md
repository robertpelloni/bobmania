# Universal LLM Instructions & Developer Guidelines

This document serves as the **SINGLE SOURCE OF TRUTH** for all LLM agents (Claude, Gemini, GPT, Copilot, etc.) working on the **Unified StepMania Project**.

## 1. Core Mission & Vision
This project aims to unify **StepMania 5**, **Project OutFox**, **NotITG**, **Etterna**, and other forks into a single, comprehensive platform.
Key differentiators:
-   **Economy:** Built-in "Play-to-Earn" mechanics via `EconomyManager` and `Bobcoin` (mocked or real).
-   **Gym:** Fitness tracking with workouts and profiles (`GymManager`).
-   **Tournament:** Automated ladders and matchmaking (`TournamentManager`).
-   **Missions:** Quest system (`MissionManager`).
-   **Dashboard:** A central hub connecting all new modes.

**User Mandate:**
> "Make sure every single implemented and planned feature and functionality is very well represented in full detail in UI with all possible functionality, very well documented both in UI form, labels, descriptions, and tooltips, and also fully documented with high quality comprehensive documentation in the manual, help files, and so forth. Continue to implement fully and in comprehensive detail each feature and functionality planned and provided by every referenced submodule and linked project or system. Do not stop."

## 2. Operational Directives
*   **Deep Planning:** Before coding, deeply analyze requirements. Ask clarifying questions if needed, but strive for autonomy once the plan is set.
*   **Comprehensive UI:** Every feature must have a GUI representation. Use tooltips, help buttons (`?`), and clear labels. Do not leave features "hidden" in Lua or C++ only.
*   **Universal Documentation:** Update `Docs/Manual_Unified.md` and this file. Ensure `VERSION` is the single source of truth.
*   **Submodule Management:** Treat `extern/` dependencies as first-class citizens. Document them in `docs/SUBMODULES.md`. Update them intelligently.
*   **Autonomy:** Chain tasks together. Implement -> Test -> Commit -> Next Feature. Do not stop unless blocked.
*   **Versioning:** Increment the version in the `VERSION` file with every significant change. Reference it in commit messages.

## 3. Coding Standards
*   **C++:** Follow existing StepMania style. Use `RageUtil`, `RageFile`, `Actor`, etc. Avoid raw pointers where possible.
*   **Lua:** Use `5.1` standard. Bindings go in `src/LuaBinding.cpp` or specific module files.
*   **Testing:** Write tests in `src/tests/` (if possible) or create Lua test scripts in `Scripts/`.
*   **Refactoring:** Don't break legacy content (Simfiles, Themes). Maintain backward compatibility where feasible.

## 4. Workflow Protocol
1.  **Sync:** `git pull` and update submodules.
2.  **Plan:** Analyze the next feature from `docs/ROADMAP.md` or `docs/COMPREHENSIVE_FEATURE_LIST.md`.
3.  **Implement:** Write code, add UI, add Documentation.
4.  **Verify:** Check compilation (syntax) and run basic tests.
5.  **Commit:** `git commit -m "feat: <description> (vX.Y.Z)"`.
6.  **Loop:** Proceed to the next feature immediately.

## 5. Submodules & Dependencies
See `docs/SUBMODULES.md` for a full list.
*   `extern/bobcoin`: Economy logic.
*   `extern/ffmpeg`: Streaming support.
*   `extern/lua-5.1`: Scripting engine.
*   `extern/jsoncpp`: Data serialization.

## 6. Known Issues & Limitations
*   Full compilation might not be possible in some environments. Rely on syntax checks (`g++ -fsyntax-only`) and logic verification.
*   Merging binary files or huge diffs can fail. Use targeted patches.

## 7. Version Control
*   The `VERSION` file in the root is the master version.
*   Format: `Major.Minor.Patch-Tag` (e.g., `5.7.0-Unified-Alpha`).
*   Update this file on every major feature addition.
