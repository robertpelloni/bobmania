# ROADMAP - Unified StepMania Platform

## Goal: To create the definitive, universal rhythm game platform that unifies the fragmented StepMania ecosystem.

### Milestone 1: Engine Unification & Parity (Completed)
- [x] **Complete ITGMania Parity:** Finalized engine fixes (Mine Fix, Phantom Holds).
- [ ] **Modding Depth:** Fully implement NotITG-style modding hooks and ensure high-performance rendering for complex modfiles.
- [ ] **Scoring Systems:** Implement Etterna-style Wife scoring (J4) as an engine-level option.

### Milestone 2: Economy & Connected Features (Current Phase)
- [x] **Real Bobcoin Integration:** Replaced the current `EconomyManager` mocks with a robust JSON-RPC bridge to a live `bobcoin` node.
- [ ] **Content Swarm (P2P):** Implement true P2P pack distribution using `libtorrent`.
- [ ] **Unified Profile Sync:** Cloud synchronization for player profiles and assets (Backend Ready, UI Integrated).

### Milestone 3: Competitive & Fitness (Next)
- [ ] **Automated Tournaments:** In-engine bracket generation and result reporting.
- [ ] **Spectator Hub:** High-performance live spectating with low latency.
- [x] **Real-world Fitness Integration:** Heart Rate Driver infrastructure (WinRT/BlueZ) implemented.

### Milestone 4: VR & Advanced UI
- [ ] **First-Class VR Support:** Complete the `ArchHooks_VR` implementation.
- [ ] **UI Modernization:** Transition to a GPU-accelerated interface.
