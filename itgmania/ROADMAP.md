# Project Roadmap

## Completed Features & Fixes
- **Maintenance**: Merged all feature branches and updated all submodules to latest upstream versions.
- **Documentation**: Created Project Dashboard and updated Changelog/Version tracking.
- **Routine Mode**: Fixed crash in Practice Menu when selecting Routine mode.
- **Editor**: Fixed issue where Editor would get stuck in Play mode without scrolling.
- **Language Support**: Fixed Czech language detection on macOS.
- **Audio Sync**: Fixed audio desync bugs (`fix-desync-bug` branch).
- **Linux Hardware Support**: Added support for PacDrive and USB drivers on Linux (`linuxpacdrive` branch).
- **VBR MP3 Support**: Fixed seeking accuracy for VBR MP3s in the editor.
- **Hidden Songs Management**: Fixed compilation issues in `ScreenManageHiddenSongs`.
- **Infrastructure**:
    - Established `VERSION.md` as the single source of truth.
    - Created `CHANGELOG.md`.
    - Created `Docs/ProjectDashboard.md`.
    - Standardized LLM instructions.

## In Progress
- **Code Cleanup**: Merging `no-global.h` (removing `StdString.h` dependency) - *Blocked by conflicts*.

## Backlog (From Known Issues)
### Critical
- **Course Editor**: Currently broken and needs a rewrite.
- **Scoring System**: Needs to be decoupled from themes and made global.
- **Routine Mode**:
    - Jukebox crashes when showing Routine steps.

### High Priority
- **Attacks**: Attacks involving noteskins do not work.
- **Graphics**: 3D models cause crashes on some graphics cards.

### Medium Priority
- **Fonts**: Japanese font maps are too large (need splitting).
- **Timing**: Split Timing needs adjustments.
- **C-Mods**: Not fully tested with new segments (Warp segments ignored).

### Low Priority
- **Unlock Code**: Old unlock code (Previews 2-4) seems broken.
- **Rendering**: Hold/Roll heads draw under preceding taps incorrectly.
