# ITGMANIA KNOWLEDGE BASE

**Context:** Rhythm Game Engine (C++)
**Parent:** [Workspace Root](../AGENTS.md)
**Master Instructions:** [LLM_INSTRUCTIONS.md](LLM_INSTRUCTIONS.md) (if exists)

## OVERVIEW
Advanced cross-platform rhythm game engine (StepMania 5.1 fork).

## STRUCTURE
```
itgmania/
├── src/             # Core C++ engine code
├── Docs/            # Documentation
├── Songs/           # Content
├── Themes/          # UI Themes (Lua)
└── NoteSkins/       # Note asset definitions
```

## CONVENTIONS
- **Build**: CMake.
- **Languages**: C++, Lua (Theming/Scripting).
- **Platform**: Windows, macOS, Linux.

## ANTI-PATTERNS
- **NO** breaking 64-bit compatibility.
- **NO** committing binary assets without verification.
