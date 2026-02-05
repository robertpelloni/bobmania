# Changelog

All notable changes to this project will be documented in this file.

## [1.3.0] - 2025-12-27

### Added
- Merged all feature branches (`beta`, `fix-desync-bug`) into `main`.
- Created `Docs/ProjectDashboard.md` listing all submodules and project structure.

### Changed
- Updated all submodules to their latest upstream versions.
- Merged upstream changes for all dependencies.
- Cleaned up submodule configuration (removed unmapped `Themes/Simply-Love-SM5`).

## [1.2.8] - 2025-12-27

### Changed
- Re-enabled C and V keys in the Editor (EditMode_Full) for adding step mods and opening the step attack menu.

## [1.2.7] - 2025-12-27

### Changed
- Increased default `MaxTextureResolution` to 4096 (from 2048) to prevent large font textures (like Japanese fonts) from being downscaled, improving load times and visual quality.

## [1.2.6] - 2025-12-27

### Fixed
- Fixed "Routine Mode Jukebox Infinite Loop" where the game would hang if no songs were available for the Routine style.
- Fixed "Unlock Code" functionality. Unlock codes defined in metrics (e.g., `Unlock1Command=...;code,"Left,Right"`) now work correctly on the Title Menu.

## [1.2.5] - 2025-12-27

### Fixed
- Fixed crash when entering Practice Mode (ScreenEdit) with a Routine style (2 players shared sides).
- Fixed Editor getting stuck in Play/Record mode if the music stops or fails to start.

## [1.2.4] - 2025-12-25

### Fixed
- Fixed language detection on macOS (specifically for Czech and other languages) by using `CFStringGetCString` instead of `CFStringGetCStringPtr`.

## [1.2.3] - 2025-12-25

### Added
- Merged `linuxpacdrive` branch to add PacDrive support on Linux.
- Added `LightsDriver_LinuxPacDrive` and `USBDriver_Impl_Libusb`.
- Added `SetSongs` method to `MusicWheel` to support `ScreenManageHiddenSongs`.

### Fixed
- Fixed compilation issue in `ScreenManageHiddenSongs` by implementing missing `MusicWheel::SetSongs`.
- Fixed VBR MP3 seeking accuracy in the editor by forcing hard seeks when `AccurateSync` is enabled.
- Optimized `RageSoundReader_MP3` seeking to use the internal seek table (TOC) for faster jumps.

## [1.2.1] - 2025-12-25

### Added
- Merged `fix-desync-bug` branch to fix audio desync issues.
- Added `VERSION.md` as the single source of truth for versioning.
- Updated `CMake/SMDefs.cmake` to read version from `VERSION.md`.

### Changed
- Updated submodules to latest versions.
- Merged upstream changes.

## [1.2.0] - Previous Release
- Initial beta release.
