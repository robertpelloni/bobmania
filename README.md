# bobmania

Robert Pelloni's fork of **StepMania**, the open-source rhythm game engine — part of his dance-game workbench alongside his ITGmania fork (`robertpelloni/itgmania`), the Fitness Center Dance Machine (`robertpelloni/fcdm`), and Simply Love.

## Status

**Actively hacked on; no packaged release.** Version in tree: v5.172.0. Last commit: 2026-11-05 (merge of the NotITG Spline Editor integration). This is a working fork with real local modifications, not a clean mirror — but there is no bobmania-specific build documentation yet. For building, follow upstream StepMania's docs ([github.com/stepmania/stepmania](https://github.com/stepmania/stepmania)).

## What's changed from upstream

Per `CHANGELOG.md` (recent entries, Nov 2026):

- **NotITG Spline Editor** (v5.172.0) — `ScreenSplineEditor` overlay for chart authors to visually inspect and modify 3D NotePath spline control points, with theme metrics and Lua syntax verification in `src/tests/run_tests.sh`.
- **Unified Dashboard** (v5.171.0) — `ScreenUnifiedDashboard` re-architected as a single-page command center grouping 13 modules (Arcade, Gym & Fitness, Tournament Ladder, NetRoom, Marketplace, Wallet Ledger, Missions, Cloud Asset Sync, Spectator TV, Replay Theater, P2P Content Swarm, Settings, Exit).
- **Gym mode** — heart-rate monitoring (`HeartRateManager`, `IHeartRateDriver`, mock driver) with a live heart-rate workout screen.
- **Economy / Bobcoin** — `EconomyManager` wired toward a Bobcoin RPC bridge; still stubs, not a live integration.
- **P2P content swarm** — `ContentSwarmManager` singleton stub mimicking a libtorrent session; not a working swarm.

## What's not done

- No bobmania-specific build docs or release packaging.
- Bobcoin integration and the P2P swarm are stubs, not working features.
- The tree also carries bulk "Protocol" automation commits (Oct 2026) tracking development/workspace state.

## Docs in this repo

`VISION.md`, `HANDOFF.md`, `CHANGELOG.md`, `Docs/` (including `Docs/DASHBOARD.md` and `Docs/Manual_Unified.md`).
