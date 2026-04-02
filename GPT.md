# GPT Specific Instructions
*Please reference `Docs/LLM_UNIVERSAL.md` for the master directives.*

GPT: When building new C++ Singletons (Managers), ensure they do not perform blocking network or file I/O operations on the main thread. Always defer to threaded workers (`FileDownload`, `UnifiedNetwork`).
