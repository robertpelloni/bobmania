# Universal LLM Instructions

This file contains the master instructions for all AI agents (Claude, Gemini, GPT, Copilot, etc.) working on the ITGmania project.

## 1. Core Philosophy
- **Autonomy**: Proactively identify and fix issues. Don't wait for confirmation if the path is clear.
- **Quality**: Adhere to strict coding standards. Prefer robust, long-term fixes over quick hacks.
- **Documentation**: Documentation is code. Keep it in sync.

## 2. Project Structure & Versioning
- **Version Control**: The single source of truth for the version is `VERSION.md`.
    - Format: `Major.Minor.Patch` (e.g., 1.2.6).
    - **Rule**: Every build/release MUST increment this version number.
    - **Action**: When bumping version, update `VERSION.md`, `CHANGELOG.md`, and ensure the git commit message references the new version (e.g., "Bump version to 1.2.6").
- **Changelog**: Maintain `CHANGELOG.md` following [Keep a Changelog](https://keepachangelog.com/en/1.0.0/).
    - Sections: `Added`, `Changed`, `Deprecated`, `Removed`, `Fixed`, `Security`.
    - Always include the version number and date for releases.

## 3. Development Protocol
1.  **Analysis**: Before writing code, analyze the existing codebase. Use `grep_search` and `read_file` to understand context.
2.  **Implementation**:
    - Follow Google C++ Style Guide.
    - Follow Lua best practices for themes/scripts.
    - Ensure backward compatibility where possible.
3.  **Verification**: Verify fixes. If you can't run the game, verify logic via code analysis.
4.  **Documentation**: Update `Docs/` if behavior changes.
5.  **Git Operations**:
    - Commit often.
    - Message format: `[Category] Short description`.
    - Sync submodules if necessary.

## 4. Submodule Management
- This project relies heavily on submodules (Themes, NoteSkins, etc.).
- **Dashboard**: Maintain `Docs/ProjectDashboard.md`. This file MUST list:
    - All submodules.
    - Their current version/commit hash.
    - Their location in the directory structure.
    - A brief description of their purpose.
- **Updates**: When updating a submodule, update the Dashboard.

## 5. Agent-Specific Instructions
(Agents should append their specific instructions below this line if needed)

### Claude
- Focus on high-level architectural consistency.

### Gemini
- Leverage large context window for deep code analysis.

### GPT
- Focus on idiomatic code generation and documentation.

### Copilot
- Integrate tightly with VS Code environment.

## 6. Handoff Protocol
- When finishing a session, update `handoff_YYYY_MM_DD.md`.
- Summarize:
    - What was done.
    - What is in progress.
    - What is next.
    - Any context the next agent needs.
