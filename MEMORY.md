# Architecture & Design Preferences

## Manager Pattern
The project uses the Singleton Manager pattern for all high-level features.
- `ECONOMYMAN`: Handles Bobcoin, marketplace, and inventory.
- `GYMMAN`: Handles workout generation and health tracking.
- `HRMAN`: Handles heart rate monitor connectivity.
- `TOURNAMENTMAN`: Handles ladders and brackets.
- `SWARMMAN`: Handles P2P content distribution.
- `MISSIONMAN`: Handles persistent goals and rewards.
- `REPLAYMAN`: Handles input recording and playback.

## Lua Binding Standards
All managers must expose their state via `Luna<T>` bindings.
- Use `ADD_METHOD` for actions.
- Use `Broadcast` via `MESSAGEMAN` to notify the UI of state changes (e.g., `BalanceChanged`, `DownloadComplete`).

## Data Persistence
- Core settings go into `Data/Static.ini`.
- Player-specific data goes into the Profile directory (`/Save/LocalProfiles/...`).
- Global persistent state (like the Economy ledger) goes into `/Save/Economy.xml`.

## Versioning
Version is sourced from `VERSION.md` at the root. Do not hardcode version strings in C++.
