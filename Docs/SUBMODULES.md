# Submodules and External Dependencies

This document tracks all git submodules, external libraries, and dependencies used within the Unified StepMania project, detailing their locations, versions, and architectural purpose.

## Git Submodules

### 1. Bobcoin
*   **Path:** `extern/bobcoin`
*   **URL:** [Assuming standard GitHub URL or local path]
*   **Version/Commit:** Tracked via git submodule. (Currently pending update).
*   **Description:** The official implementation of the Bobcoin cryptocurrency node and wallet.
*   **Usage:** Integrated via `src/Economy/Bridge/BobcoinBridge.cpp` to handle real-world transactions, player balances, and mining reward distributions. It replaces local XML-based economy tracking.

### 2. Simply-Love-SM5
*   **Path:** `Themes/Simply-Love-SM5`
*   **URL:** https://github.com/Simply-Love/Simply-Love-SM5
*   **Version/Commit:** Tracked via git submodule.
*   **Description:** The premier, highly polished theme for StepMania 5.
*   **Usage:** Serves as the baseline for high-performance visual themes. We maintain strict Lua 5.1 compatibility to ensure this theme (and community modifications of it) run flawlessly on the Unified engine.

## External Libraries (Vendored in `extern/`)

### 1. Lua
*   **Version:** 5.1.5
*   **Path:** `extern/lua-5.1/`
*   **Usage:** The absolute core of the StepMania UI frontend. We rigidly enforce the 5.1 standard to maintain compatibility with 20 years of community themes.

### 2. JsonCpp
*   **Version:** 1.9.5 (approximate, embedded)
*   **Path:** `extern/jsoncpp/`
*   **Usage:** Used for parsing `Data/MarketplaceCatalog.json`, handling web requests, and processing JSON RPC responses from the Bobcoin node and matchmaking servers.

### 3. Zlib
*   **Version:** 1.2.11 (approximate, embedded)
*   **Path:** `extern/zlib/`
*   **Usage:** Essential for compressing/decompressing `.smzip` packages, network streams, and `ContentSwarmManager` chunk validation.

## Monorepo Workflow & Submodule Management

Unified StepMania treats submodules as active development zones, not just static dependencies.

1.  **Direct Editing:** When working on a feature that crosses into a submodule (e.g., adding an RPC endpoint to Bobcoin), edit the files directly inside `extern/bobcoin/`.
2.  **Commit & Push:** Commit those changes inside the submodule's directory and push to its respective remote repository.
3.  **Update Parent:** Once the submodule is pushed, return to the root directory, `git add extern/bobcoin`, and commit the pointer update to the main StepMania repository.

*Note: Due to GitHub Actions CI size limits and environment constraints, `git submodule update --init --recursive` may fail in isolated agent sessions. In those cases, document the required changes and use local mocks until a full deployment sync can occur.*
