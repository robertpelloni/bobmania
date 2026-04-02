# Unified StepMania: The Definitive User Manual

Welcome to **Unified StepMania**, the culmination of the rhythm gaming community's efforts to integrate features from various forks (StepMania 5, OutFox, NotITG, Etterna) into a single, cohesive, and feature-rich platform.

This manual serves as the comprehensive guide to every system, mode, and interface within the game.

---

## Table of Contents

1. [Introduction to Unified Architecture](#1-introduction-to-unified-architecture)
2. [The Unified Dashboard](#2-the-unified-dashboard)
3. [Arcade & Core Gameplay](#3-arcade--core-gameplay)
4. [Gym & Fitness Tracking](#4-gym--fitness-tracking)
5. [Economy, Marketplace & Mining](#5-economy-marketplace--mining)
6. [Tournament & Competitive Play](#6-tournament--competitive-play)
7. [Missions & Progression](#7-missions--progression)
8. [Content Swarm & Asset Sync](#8-content-swarm--asset-sync)
9. [Spectator, Replays & Network](#9-spectator-replays--network)
10. [Advanced Settings & Under the Hood](#10-advanced-settings--under-the-hood)

---

## 1. Introduction to Unified Architecture

Unified StepMania abandons the fragmented "fork" model. By introducing global "Managers" accessible via both the C++ backend and the Lua frontend, the game supports highly complex systems without sacrificing performance.

*   **Global Singletons:** Systems like `ECONOMYMAN` (Economy), `GYMMAN` (Gym), and `SWARMMAN` (Content Delivery) run persistently in the background.
*   **Lua Overlays:** The UI is driven entirely by Lua 5.1 scripts located in `Themes/default/BGAnimations/`. This allows rapid theme development.
*   **The Help System:** Pressing `SELECT` on any custom menu brings up the global `HelpOverlay`, providing context-sensitive tooltips for every feature.

---

## 2. The Unified Dashboard

Upon launching the game (and bypassing the traditional Title Menu), you arrive at the **Unified Dashboard**. This is your central hub.

### Features:
*   **Arcade:** Launches the traditional song wheel.
*   **Gym:** Enters the fitness portal.
*   **Tournament:** Accesses the competitive ladder.
*   **Economy:** Opens your wallet and the marketplace.
*   **Missions:** Accesses the quest log.
*   **Swarm Network:** Opens the P2P content discovery interface.
*   **NetPlay:** Connects to standard StepMania Online (SMO) lobbies.

### Controls:
*   `Up`/`Down`: Navigate the menu.
*   `Start`: Enter the selected mode.
*   `Select`: Toggle the context-aware Help Overlay.

---

## 3. Arcade & Core Gameplay

The Arcade mode retains the classic StepMania experience but includes several under-the-hood enhancements imported from other forks.

### Modifiers & Options (Accessible via `Start` after song selection)
*   **Judge Scale:** Tighten timing windows (J1-J8) for Etterna-style accuracy practice.
*   **NotITG Parity:** Full support for Lua-driven visual modifications, custom NotePaths, and viewport manipulation (`fov`, `vanishpoint`).
*   **Visual Aids:** Screen Filter (background dimming) and ProTiming (millisecond early/late indicators) are built-in.
*   **Practice Mode:** Forces all successful hits to "Marvelous" (`W1`) to train timing consistency.
*   **Ghost Tapping:** Toggle the penalty for pressing buttons when no arrow is present.

---

## 4. Gym & Fitness Tracking

The Gym mode transforms StepMania into a dedicated fitness tool.

### Gym Dashboard (`ScreenGymWelcome`)
*   **Profile:** Displays your configured Weight, Height, and calculated BMI.
*   **Stats:** Tracks your "Daily Calorie Goal," "Total Calories Burned," and "Login Streak."

### Workout Generation
*   Instead of manually selecting songs, you configure a workout duration (e.g., 15 minutes) and a difficulty range (e.g., Meters 5-8).
*   The `GymPlaylistGenerator` builds a continuous course with a warm-up, peak intensity, and cool-down phase.

### Workout HUD (`ScreenGymWorkout`)
*   **Live Metrics:** Displays current song, remaining time, and real-time calorie burn.
*   **Heart Rate Monitor:** Integrates with the `HeartRateManager`. If a compatible hardware device is connected (or simulated), your live BPM is displayed on-screen, pulsing when elevated.

---

## 5. Economy, Marketplace & Mining

Unified StepMania features a fully integrated virtual economy powered by **Bobcoin**.

### Wallet & History (`ScreenWalletHistory`)
*   View your total Bobcoin balance.
*   Review a comprehensive transaction ledger detailing mining rewards, tips sent/received, and marketplace purchases.

### Mining by Dancing
*   The game rewards physical effort. Completing songs grants Bobcoin based on your score percentage, the song's difficulty, and your underlying "Hashrate."
*   *Formula snippet:* `Reward = (Score % * Difficulty * Base) + Hashrate`

### The Marketplace (`ScreenMarketplace`)
*   Spend Bobcoin to unlock content dynamically.
*   **Song Packs:** Download new charts instantly.
*   **Themes/Skins:** Purchase visual overhauls (e.g., UI changes, NoteSkins).
*   **Boosts/Hardware:** Purchase virtual "Miners" to permanently increase your Hashrate.

---

## 6. Tournament & Competitive Play

The Tournament module facilitates structured, high-stakes competition.

### The Ladder (`ScreenTournamentLadder`)
*   View the global ELO rankings.
*   Challenge players asynchronously or join scheduled live brackets.

### Betting & Wagering (`ScreenBettingEntry`)
*   Spectators and participants can wager Bobcoin on match outcomes, integrating the Economy with competitive play.

---

## 7. Missions & Progression

*   Access `ScreenMissionSelect` to view daily, weekly, and lifetime objectives.
*   **Examples:** "Burn 500 Calories," "Achieve a AAA grade on a Level 10," "Mine 50 Bobcoin."
*   Completing missions grants XP, unique titles, and Avatar Frames usable in online lobbies.

---

## 8. Content Swarm & Asset Sync

Say goodbye to manually downloading `.zip` files from forums.

### Swarm Network (`ScreenContentNetwork`)
*   Powered by the `ContentSwarmManager`, this acts as a Peer-to-Peer tracker.
*   Browse community-uploaded packs, see live seeder counts, and download content directly into your `Songs/` folder.

### Asset Synchronization (`ScreenAssetSync`)
*   Connects your local client to a central cloud server.
*   Ensures your unlocked marketplace items, high scores, and gym profile are synchronized across multiple arcade cabinets or home PCs.

---

## 9. Spectator, Replays & Network

### Spectator Mode (`ScreenSpectate`)
*   Watch live matches currently happening on the network.
*   Includes a live chat interface and the ability to "Cheer" (donate Bobcoin) to the performing player.

### Unified Chat (`ScreenNetRoom`)
*   A robust IRC-style chat room overlaying the multiplayer lobby, powered by the `ChatManager`.

### Replays
*   Plays are automatically saved.
*   Use the Replay Browser to load past performances, enabling "Ghost" play where you can race against your past self or a downloaded pro run.

---

## 10. Advanced Settings & Under the Hood

*   **Discord RPC:** Rich presence integration broadcasts your current song, difficulty, and game mode to Discord.
*   **VR Rendering:** Experimental hooks (`m_bVRMode`) exist for side-by-side rendering, laying the groundwork for future VR headset support.
*   **Security:** Economy save files (`Economy.xml`) are cryptographically signed using SHA-256 to prevent tampering.

---
*Manual Version: 5.7.2 (Beta Polish Phase)*
*Compiled by: The Unified Development AI*
