# bobmania

Robert Pelloni's fork of **StepMania**, the open-source rhythm game engine — part of his dance-game workbench alongside his ITGmania fork (`robertpelloni/itgmania`), the Fitness Center Dance Machine (`robertpelloni/fcdm`), and Simply Love.

## Status

**Engine restored 2026-11-19; no packaged release.** Version in tree: v5.172.0 (`VERSION` file).

What happened, in plain terms:

- On 2026-10-19, commit `17f666c8` ("Protocol #261: track code, agent-memory/sessions, docs/config") replaced this branch's tree with Git's **empty tree** — 0 additions, 1,980,932 deletions. The entire StepMania engine was deleted from `master`.
- The Oct–Nov 2026 commits that followed re-added only a 57-file overlay (docs, two Lua theme overlays, validator scripts). For a month, `master` contained no engine source at all, while its docs described a working fork.
- On 2026-11-19, `master` was restored from `f86ecf13` (2026-07-18, the last full tree before the wipe: 9,457 entries), with the November overlay grafted on top (Unified Dashboard + Spline Editor Lua overlays, theme metrics/localization, `scripts/validate_lua.py` + `scripts/audit_dashboard.py`, current docs, `VERSION` v5.172.0).
- The pre-restore overlay line is preserved on the branch `overlay-snapshot-2026-11-19` (head `a80903eb`).

Build verification: **Not yet — configure fails, and the breakage predates the October wipe.** Verified by Muse on the restored tree (Ubuntu, CMake 3.28, GCC 13): `src/tests/run_tests.sh` passes its real checks (Lua syntax validation of both overlays; dashboard audit finds all 13 modules), but `cmake` configure does not complete. In the order hit: (1) `StepmaniaCore.cmake` requires the Xtst, Xinerama, and udev packages, but `CMake/Modules/` ships no `FindXtst.cmake`, `FindXinerama.cmake`, or `Findudev.cmake`, and the Ubuntu dev packages ship no CMake config files for them; (2) the NASM/YASM gate tests `NASM_FOUND`/`YASM_FOUND`, which nothing in this tree ever sets — configure fails there even with NASM installed; (3) with those bypassed, `configure_file` inputs `src/config.in.hpp` and `src/verstub.in.cpp` are missing from the tree (absent at the July restore source and at the July 10 ghost-replay commit alike), `extern/config.pcre.in.h` and `extern/pcre/pcre.h.generic` are also missing, and `src/CMakeLists.txt` produces a parse error. The July 2026 ghost-replay PR already noted the CMake build was "structurally broken"; this restore confirms it with specifics. Fixing the build is follow-up work, not part of the restoration.

## What's in the tree

The full StepMania 5.1-derived engine (`src/`, `extern/`, `Themes/`, `NoteSkins/`, root `CMakeLists.txt`), plus this fork's own work, plus a nested `itgmania/` tree and a `server/` directory that are tracked workspace state from the July 2026 checkpoint — they are part of the restored tree as-is, not a curated layout.

## What's changed from upstream

Per `CHANGELOG.md` and the restored `src/` tree:

- **Unified Dashboard** (v5.171.0) — `ScreenUnifiedDashboard` Lua overlay re-architected as a single-page command center grouping 13 modules (Arcade, Gym & Fitness, Tournament Ladder, NetRoom, Marketplace, Wallet Ledger, Missions, Cloud Asset Sync, Spectator TV, Replay Theater, P2P Content Swarm, Settings, Exit).
- **NotITG Spline Editor** (v5.172.0) — `ScreenSplineEditor` Lua overlay for inspecting and modifying 3D NotePath spline control points, with theme metrics and localization.
- **Gym mode** — `HeartRateManager` / `GymManager` in C++, with Mock, BlueZ, and WinRT heart-rate drivers. The Mock driver is the one exercised in practice; the hardware drivers are untested.
- **Economy / Bobcoin** — `EconomyManager` with a `BobcoinBridge` toward a Bobcoin JSON-RPC node. Stub-level: not a live, verified chain integration.
- **P2P content swarm** — `ContentSwarmManager` is a structural stub mimicking a libtorrent session; it is not a working swarm.
- **Ghost replay** — `Player.cpp` support for rendering a ghost `NoteField` from recorded replay input (merged July 2026; its own PR noted the CMake build was structurally broken out of the box, so treat compilation as unverified until a build says otherwise).

## What's not done

- No bobmania-specific build documentation or release packaging. For building, follow upstream StepMania's docs ([github.com/stepmania/stepmania](https://github.com/stepmania/stepmania)) using the root `CMakeLists.txt`.
- `src/tests/run_tests.sh` runs exactly two real checks: Lua syntax validation of the two overlays, and the dashboard routing audit. It previously printed `PASS` for the C++ managers without running anything; those lines now read NOT TESTED, because there are no automated C++ manager tests in this repo.
- Bobcoin integration and the P2P swarm are stubs, not working features (see above).

## Docs in this repo

`VISION.md` (aspirational design — not a statement of what is built), `HANDOFF.md` (current state, including the 2026-11-19 restore), `CHANGELOG.md`, `Docs/` (including `Docs/DASHBOARD.md` and `Docs/Manual_Unified.md`).
