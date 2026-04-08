<<<<<<< HEAD
See `docs/LLM_UNIVERSAL.md`.
See [LLM_INSTRUCTIONS.md](LLM_INSTRUCTIONS.md) for universal instructions.

**IMPORTANT:**
Please refer to `docs/LLM_UNIVERSAL.md` for the latest and most comprehensive instructions, project goals, and coding standards.

This file is kept for historical context but the active directives are in `docs/LLM_UNIVERSAL.md`.
=======
# GPT Specific Instructions
*Please reference `Docs/LLM_UNIVERSAL.md` for the master directives.*

GPT: When building new C++ Singletons (Managers), ensure they do not perform blocking network or file I/O operations on the main thread. Always defer to threaded workers (`FileDownload`, `UnifiedNetwork`).
>>>>>>> origin/unified-ui-features-13937230807013224518
