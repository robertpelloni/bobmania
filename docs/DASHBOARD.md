# Project Dashboard & Submodule Status

## Project Structure
This project is a fork of StepMania 5.1 (New), integrating features from ITGMania and a custom Economy/Tournament system.

### Directory Layout
*   **`src/`**: Core C++ source code.
    *   **`Economy/`**: New EconomyManager, Wallet, and Marketplace logic.
    *   **`Tournament/`**: Tournament ladder and QR login logic.
    *   **`Unified/`**: Unified Dashboard UI screens.
    *   **`Gym/`**: Gym mode logic.
*   **`Themes/`**: Lua-based themes.
    *   **`default/`**: The primary theme, updated with Dashboard links.
*   **`Docs/`**: Documentation.
*   **`extern/`**: External dependencies (Submodules).

## Submodules
*   *(No submodules currently registered in this fork. Dependencies are vendored or managed via CMake.)*

## Build Information
*   **Current Version:** 5.1.0
*   **Build System:** CMake
*   **Primary Branch:** `5_1-new`

## Recent Changes
*   **Economy Integration:** `ScreenGameplay` now supports wallet tracking and betting.
*   **Dashboard:** Unified Dashboard accessible from Title Menu.
*   **NotITG:** Render Targets and Shaders supported; NotePath pending.
