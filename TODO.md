# TODO - Immediate Tasks & Bug Fixes

## 1. Engine Stability & Bug Fixes
- [x] **Overlapping Hold Bug:** Implemented phantom hold fix from ITGmania.
- [x] **Mine Fix Verification:** Integrated DinsFire64's mine fix.
- [ ] **Memory Management:** Audit the use of `SAFE_DELETE` vs modern smart pointers in the new Manager classes.

## 2. Feature Completion (Replacing Mocks)
- [x] **Economy Backend:** Implemented JSON-RPC communication in `BobcoinBridge.cpp`.
- [x] **Heart Rate Driver:** Infrastructure for `WinRT` and `BlueZ` drivers created.
- [x] **Replay Ghosting:** Implemented "Ghost NoteField" in `Player.cpp`.

## 3. UI & UX Improvements
- [x] **Marketplace Polishing:** Enhanced item list fetching and feedback.
- [x] **Asset Sync Feedback:** Real progress and status reporting implemented.
- [x] **Unified Dashboard:** All major shortcuts wired and functional.

## 4. Documentation & Versioning
- [x] **Centralized Versioning:** Engine pulls version string from `VERSION.md`.
- [ ] **API Documentation:** Generate up-to-date Lua API documentation for all new `MANAGER` singletons.
