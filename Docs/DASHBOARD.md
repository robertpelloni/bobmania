# Unified StepMania - Unified Dashboard Architecture

## Overview
`ScreenUnifiedDashboard` is the primary centralized command center of Unified StepMania. It combines and consolidates all subpage functions into a singular layout organized by high-value prominent placement, completely eliminating fragmented navigation menus.

## Consolidated Hierarchy & Categorization

### 1. CORE RHYTHM & FITNESS (High-Value Prominence)
- **Arcade Mode (`ScreenSelectMusic`)**: Standard rhythm gameplay; quick access to music wheel, difficulty selection, and play modes.
- **Gym & Fitness (`ScreenGymWelcome`)**: Calorie burn counters, workout routines, duration trackers, and real-time heart-rate polling (`HEARTRATEMAN`).
- **Tournament Ladder (`ScreenTournamentLadder`)**: Ranked competitive ladder standings, ELO score tracking, and automated tournament matchmaking (`TOURNAMENTMAN`).
- **Online NetRoom (`ScreenNetRoom`)**: Multiplayer lobby rooms, chat box, room wheel, and direct remote server connections (`UNIFIED_NET`).

### 2. ECONOMY & PROGRESSION
- **Marketplace (`ScreenMarketplace`)**: Catalog of unlockable custom songs, noteskins, and visual mods purchasable using Bobcoin rewards earned by dancing.
- **Wallet Ledger (`ScreenWalletHistory`)**: Bobcoin transaction ledger, RPC node block height confirmation, balance inspection (`ECONOMYMAN`).
- **Missions & Quests (`ScreenMissionSelect`)**: Daily rhythm and fitness mission objectives with reward disbursements upon stage pass (`MISSIONMAN`).
- **Cloud Asset Sync (`ScreenAssetSync`)**: Decentralized cloud synchronization for local user profiles, high scores, and marketplace unlocks (`ASSETSYNCMAN`).

### 3. MEDIA & SYSTEM TOOLS
- **Spectator TV (`ScreenSpectate`)**: Real-time spectator broadcasting for tournament matches and live high-level community performances.
- **Replay Theater (`ScreenReplayMenu`)**: Ghost replay recording, playback inspection, millisecond offset visualization (`REPLAYMAN`).
- **P2P Content Swarm (`ScreenContentNetwork`)**: Decentralized BitTorrent/swarm downloader for community `.smzip` pack sharing and automated caching.
- **Unified Settings (`ScreenUnifiedOptions`)**: Advanced engine toggles including Ghost Tapping, Both At Once, FOV overrides, and VR display rendering.
- **Exit / Server Shutdown (`ScreenExit`)**: Clean shutdown workflow that terminates local RPC services, closes WebSockets, and safely exits the engine.

## System Tray & Background Service Architecture
*   **System Tray Integration:** When running in desktop environment mode, Unified StepMania and its helper daemon operate an OS system tray / notification icon. The tray menu provides instant server status introspection (WebSocket connection, RPC node sync status), quick mute, and a dedicated "Quit Servers & Exit" command that safely stops background services without killing process state abruptly.
*   **Daemon & Server Process Management:** Background network processes (the Bobcoin RPC daemon bridge, WebSocket matchmaking proxy) are monitored. If a background service encounters network loss, status tags in the dashboard reflect the degraded state dynamically.

## Context-Sensitive Help & Tooltips
- Every item features a prominent category tag, distinct description, and context-sensitive help box.
- Pressing `SELECT` invokes the global `HelpOverlay`, presenting operational guidance and keybindings without leaving the dashboard.
