# Ideas for Economy Module Improvements

## Refactoring
- [ ] **Abstract Currency:** The current system relies heavily on "Bobcoin" (integers). It should be abstracted to support multiple currencies (Credits, Gems, etc.) via a `CurrencyManager`.
- [ ] **Catalog Provider:** Move JSON parsing logic out of `EconomyManager` into a `CatalogProvider` class to support multiple sources (local file, remote API).

## Features
- [ ] **P2P Trading:** Allow players to trade items directly using `UnifiedNetwork`.
- [ ] **Auction House:** A persistent marketplace for player-sold items.
- [ ] **Staking:** "Lock" Bobcoin to earn interest over time (simulated DEFI).

## Technical Debt
- [ ] **Sync:** `Save/Economy.xml` write is blocking. Move to a background thread.
- [ ] **Security:** Signature check is weak (SHA256 of plain text). Should encrypt the entire file.
