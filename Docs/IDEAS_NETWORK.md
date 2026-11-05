# Ideas for Network & Tournament Improvements

## Refactoring
- [ ] **Protocol Buffers:** Replace custom JSON protocol with Protobuf for efficiency and type safety.
- [ ] **State Machine:** `TournamentManager` logic is procedural. Refactor to a proper State Machine (Idle -> Matching -> Picking -> Playing -> Reporting).

## Features
- [ ] **Spectator Delay:** Add configurable delay to `SpectatorManager` to prevent stream sniping.
- [ ] **Replay Streaming:** Stream replay data chunks in real-time instead of just video.
- [ ] **Team Battles:** 2v2 or 4v4 tournament modes.

## Technical Debt
- [ ] **Mock Server:** `mock_server.js` is a single file. Needs splitting into modules (Auth, Matchmaking, Chat).
- [ ] **Error Handling:** Network disconnects during a match are not gracefully handled (currently just logs error).
