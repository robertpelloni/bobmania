# Release Notes - StepMania 5.1.2 (Economy & Gym Update)

## Overview
This release introduces a comprehensive Economy System, Gym Mode for fitness tracking, and achieves full NotITG parity for advanced content support.

## New Features

### 1. Economy System
*   **EconomyManager**: A centralized system for managing virtual currency ("Coins"), transactions, and assets.
*   **Wallet**: Persistent player wallet tracking earnings from gameplay and mining.
*   **Marketplace**: Buy and sell assets (Songs, Skins, Shares) using earned coins.
*   **Betting**: Wager coins on gameplay performance in "Wager Battle" mode.
*   **Server Node**: Idle game mode where the client simulates distributed computing to earn passive income.
*   **DAO Governance**: Vote on game proposals using "Company Shares".

### 2. Gym Mode
*   **GymPlaylistGenerator**: Automatically generates workout circuits based on target duration and intensity (Light, Moderate, Vigorous, Pro Athlete).
*   **Calorie Tracking**: Real-time calorie estimation based on song difficulty and duration.
*   **CRS Export**: Save custom workouts to `.crs` files for later use.
*   **Intensity Selector**: Choose your workout intensity in the Gym Welcome screen.

### 3. NotITG Parity
*   **NotePath**: Lua API (`PlayerState::SetNotePath`) allows arbitrary function-based control of note positions (X, Y, Z).
*   **Modfiles**: Automatic loading of `mods.lua` from song directories for per-song scripted effects.
*   **Shaders**: Full support for custom vertex and fragment shaders via Lua.
*   **Render Targets**: Support for `ActorFrameTexture` for advanced visual effects.

### 4. Unified Dashboard
*   **ScreenUnifiedDashboard**: A new central hub for accessing all new features (Arcade, Gym, Market, Wallet, etc.).
*   **Profile Customization**: Equip titles and assets purchased in the marketplace.

## Technical Improvements
*   **Pitch-Dependent Rate**: Option to enable vinyl-style pitch shifting for rate mods.
*   **Fast Profile Switching**: Quickly cycle through local profiles.
*   **Song Loading**: Dynamic loading of new songs without restarting the game.

## Balancing Changes
*   **Mining**: Increased passive rewards to 30 coins/minute.
*   **Dividends**: Adjusted shareholder payout to ~0.0005% of treasury per share per minute to prevent hyper-inflation.

## Known Issues
*   **QR Login**: Currently simulated via keyboard input for testing purposes.
*   **Assets**: Some screens use placeholder text instead of final graphics.

## Credits
*   **Merge Team**: Hyper
*   **Original Code**: StepMania Team, ITGMania Team, NotITG Team
