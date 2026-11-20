# Handoff Document

## 2026-11-19 — Engine restoration (Tier 2 fix)

- **Problem:** `master` had no engine. Commit `17f666c8` (2026-10-19, "Protocol #261") set the tree to Git's empty tree (`4b825dc6`, 0 additions / 1,980,932 deletions). Subsequent Oct–Nov commits re-added only a 57-file docs/Lua overlay, while README/HANDOFF/VISION described C++ subsystems that were not in the tree.
- **Restore source:** `f86ecf1308d8e835b28cfb18539055e17c77f24d` (2026-07-18, "chore: auto checkpoint before sync") — last full tree before the wipe, 9,457 entries.
- **Graft:** the November overlay was applied on top of the restored tree (overlay wins on conflict): Unified Dashboard + Spline Editor Lua overlays, `Themes/default/metrics.ini`, `Themes/default/Languages/en.ini`, `scripts/validate_lua.py`, `scripts/audit_dashboard.py`, `CHANGELOG.md`, current `Docs/*` versions, `VERSION` (v5.172.0), and this handoff + README.
- **Preservation:** the pre-restore overlay line is kept on branch `overlay-snapshot-2026-11-19` (head `a80903eb`). The wipe commit and everything after it remain in that branch's history.
- **Test script honesty:** `src/tests/run_tests.sh` printed `EconomyManager... PASS` etc. without running any test — in both the July tree and the November overlay. Those lines now read NOT TESTED. The script's real checks are the Lua syntax validation and the dashboard routing audit.
- **Verification performed by Muse on the restored tree:** `src/tests/run_tests.sh` run on the restored tree (f86 + full overlay graft, locally): exit 0 — Lua syntax validation PASS for both overlays; dashboard routing audit PASS (13/13 modules found, SUCCESS line); C++ managers print NOT TESTED (honest labels, no fake PASS).
- **Build verification:** CMake configure ATTEMPTED and FAILED on the restored tree (Ubuntu, CMake 3.28.3, GCC 13.3, standard StepMania dev libraries + NASM installed via apt). Stock configure dies at `find_package(Xtst REQUIRED)` (StepmaniaCore.cmake:292). Defects in order: (1) missing Find modules in `CMake/Modules/` for Xtst, Xinerama, udev (no distro CMake configs either); (2) NASM/YASM gate at StepmaniaCore.cmake:338 reads `NASM_FOUND`/`YASM_FOUND`, set nowhere in the tree — fails with NASM 2.16.01 installed; (3) bypassing 1–2 (feature flags off, `-DNASM_FOUND=1`, a scratch Findudev module outside the repo) gets through FFmpeg/OpenGL/sound detection, then fails on missing `src/config.in.hpp`, `src/verstub.in.cpp`, `extern/config.pcre.in.h`, `extern/pcre/pcre.h.generic` (all absent at f86 and at ghost-replay head eca21cf0) and a parse error in `src/CMakeLists.txt`. Conclusion: the restored tree does not configure, and the build breakage predates the 2026-10-19 wipe — it was already in the July tree. No compile was attempted past configure.
- **Not verified:** gameplay, hardware heart-rate drivers (BlueZ/WinRT), Bobcoin RPC against a live node, the nested `itgmania/` tree's build, the `server/` backend.

## Current state

- Working StepMania-derived engine tree with this fork's C++ managers (`src/Economy`, `src/Gym`, `src/Network`, `src/Tournament`, `src/Unified`), Lua dashboard/spline overlays, and the July 2026 workspace state (`itgmania/` nested tree, `server/`, `.jules/` / `.pi/` session state) restored as-is.
- Known stubs: `ContentSwarmManager` (libtorrent-shaped stub), Bobcoin bridge (RPC stub-level), Mock heart-rate driver is the exercised path.
- `Docs/TODO.md` lists the real backlog (async networking, real heart-rate hardware drivers, libtorrent integration, matchmaking persistence). Items there that describe C++ files now have their files back.

## Next steps (owner's call)

1. Get a full CMake build green on one platform and record the recipe in this repo (the ghost-replay PR in July 2026 already noted the out-of-box CMake configuration was broken).
2. Replace the NOT TESTED manager lines with at least one real automated test per manager, or delete the section.
3. Decide whether the nested `itgmania/` tree and `server/node_modules` belong in this repo at all — they are restored July state, not curated content.

---

The previous HANDOFF (a long Protocol-maintenance log claiming repeated test passes) is preserved in git history on `overlay-snapshot-2026-11-19` and in the pre-restore `master` history. It was replaced, not edited, because its claims did not describe the tree it shipped with.
