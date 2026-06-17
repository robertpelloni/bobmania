# ROADMAP - Unified StepMania Platform

## Goal: To create the definitive, universal rhythm game platform that unifies the fragmented StepMania ecosystem.

### Milestone 1: Engine Unification & Parity (Current Phase)
- [ ] **Complete ITGMania Parity:** Finalize all engine fixes and feature ports.
- [ ] **Modding Depth:** Fully implement NotITG-style modding hooks and ensure high-performance rendering for complex modfiles.
- [ ] **Scoring Systems:** Implement Etterna-style Wife scoring (J4) as an engine-level option.

### Milestone 2: Economy & Connected Features (Next)
- [ ] **Real Bobcoin Integration:** Replace the current `EconomyManager` mocks with a robust JSON-RPC bridge to a live `bobcoin` node.
- [ ] **Content Swarm (P2P):** Implement true P2P pack discovery and sharing using `libtorrent`, replacing the current HTTP download stubs.
- [ ] **Unified Profile Sync:** Real cloud synchronization for player profiles, scores, and marketplace assets.

### Milestone 3: Competitive & Fitness (Future)
- [ ] **Automated Tournaments:** In-engine bracket generation, automatic song selection, and result reporting.
- [ ] **Spectator Hub:** High-performance live spectating with low latency.
- [ ] **Real-world Fitness Integration:** Full Bluetooth LE support for heart rate monitors and integration with mobile health apps.

### Milestone 4: VR & Advanced UI
- [ ] **First-Class VR Support:** Complete the `ArchHooks_VR` implementation for a fully immersive rhythm experience.
- [ ] **UI Modernization:** Transition from legacy Lua/XML UI to a more flexible, GPU-accelerated interface while maintaining backward compatibility.
