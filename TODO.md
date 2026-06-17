# TODO - Immediate Tasks & Bug Fixes

## 1. Engine Stability & Bug Fixes
- [ ] **Overlapping Hold Bug:** Fix the logic error in `NoteData.cpp` or `Player.cpp` where overlapping holds cause judgment issues or crashes.
- [ ] **Mine Fix Verification:** Double-check `DinsFire64`'s mine fix against all edge cases in modfiles.
- [ ] **Memory Management:** Audit the use of `SAFE_DELETE` vs modern smart pointers in the new Manager classes.

## 2. Feature Completion (Replacing Mocks)
- [ ] **Economy Backend:** Implement `src/Economy/Bridge/BobcoinBridge.cpp` using `jsoncpp` to talk to a real JSON-RPC endpoint.
- [ ] **Heart Rate Driver:** Replace `HeartRateDriver_Mock` with `HeartRateDriver_WinRT` and `HeartRateDriver_BlueZ`.
- [ ] **Replay Ghosting:** Implement the "Ghost NoteField" in `Player.cpp` that renders data from `ReplayManager` in real-time.

## 3. UI & UX Improvements
- [ ] **Marketplace Polishing:** Add tooltips and detailed asset descriptions in `ScreenMarketplace`.
- [ ] **Asset Sync Feedback:** Improve the visual progress bar and status messages in `ScreenAssetSync`.
- [ ] **Unified Dashboard:** Wire up the "Mission" and "Gym" shortcuts to their respective new screens.

## 4. Documentation & Versioning
- [ ] **Centralized Versioning:** Refactor the engine to pull its version string exclusively from `VERSION.md`.
- [ ] **API Documentation:** Generate up-to-date Lua API documentation for all new `MANAGER` singletons.
