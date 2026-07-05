# Unified StepMania: Ultimate Vision & Design Philosophy

## The Ultimate Goal
The overarching goal of the Unified StepMania project is to eliminate the severe fragmentation that has plagued the rhythm gaming community for over a decade. By aggressively merging features from disparate forks—StepMania 5.1/5.2, OutFox, NotITG, and Etterna—into a single, modular, and universally compatible engine, we seek to create the definitive rhythm game platform.

This platform will serve all player archetypes simultaneously:
1.  **The Competitor (Etterna/ITG):** Seeking millisecond precision, judge scaling, strict timing windows, global ELO ladders, and robust replay systems.
2.  **The Modder (NotITG):** Requiring deep Lua shader hooks, spline-based NotePaths, arbitrary viewport manipulation, and dynamic field generation.
3.  **The Fitness Enthusiast (Gym Mode):** Needing structured workouts, heart rate monitoring, calorie tracking, and dynamic playlist generation.
4.  **The Economist (Bobcoin/Marketplace):** Engaging in a "play-to-earn" ecosystem where physical effort translates to virtual currency (Bobcoin), used to purchase themes, songs, and boosts.

## Design Philosophy

### 1. The "Manager" Paradigm
Instead of polluting the core `StepMania.cpp` and `ScreenGameplay.cpp` files with endless conditional logic, we have adopted a modular "Manager" pattern.
*   **Encapsulation:** Subsystems like Economy (`ECONOMYMAN`), Gym (`GYMMAN`), and Networking (`UNIFIED_NET`) are encapsulated within `src/` subdirectories.
*   **Global Accessibility:** These managers are instantiated globally during boot and are cleanly destroyed upon exit, allowing any other C++ class to query their state safely.
*   **Sandboxed Hooks:** Core gameplay loops only invoke these managers through lightweight hooks (e.g., calling `ECONOMYMAN->AwardMiningReward()` at the end of a song), preventing structural rot.

### 2. Deep UI Separation (Lua 5.1)
The C++ backend handles the math, the networking, and the file I/O. The Lua frontend (specifically the standard `Themes/default/` structure) handles 100% of the UI.
*   **No Hardcoded UIs:** We do not build menus in C++. All new features (Marketplace, Tournaments) are exposed to Lua via bindings (`Luna<T>`).
*   **Legacy Preservation:** By maintaining strictly Lua 5.1 compatibility, thousands of legacy themes and noteskins remain fully functional without modification.

### 3. Asynchronous by Default
Systems that interact with the outside world (Blockchain RPC, Swarm P2P Downloads, Heart Rate Bluetooth LE) must never block the main rendering thread.
*   They utilize background polling, threaded downloads (`FileDownload.cpp`), and event-driven callbacks.

## The Future State
When the Unified StepMania vision is fully realized, a player will be able to boot the game, have their profile automatically fetched from a decentralized cloud, join a live-streamed tournament bracket, wager Bobcoin on the outcome, and play a visually stunning NotITG-style chart with Etterna-strict timing—all from the exact same executable, without swapping binaries or tweaking config files.

## Protocol #74 & #76 Updates (v5.96.0 - v5.98.0)
Continuous autonomous iteration has rapidly moved the protocol up to version v5.98.0. System stability remains paramount, resisting external hallucinated feature sets and maintaining the pure C++/Lua monorepo framework.

## Protocol #83 Updates (v5.101.0)
The environment continues to advance to Protocol #83. Stability is verified by actively rejecting hallucinated requests outside the scope of the `bobmania` architecture, preserving the integrity of the project timeline.
