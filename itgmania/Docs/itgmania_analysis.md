# ITGmania Comprehensive Project Analysis

## 1. Project Overview
**ITGmania** is an advanced cross-platform rhythm game engine, originally forked from StepMania 5.1. It provides robust support for modern rhythm game features, hardware integration, and high customizability via Lua scripting. The engine is targeted towards players and arcade hardware setups running dance-based rhythm games.

### Core Philosophy
- **Autonomy & Robustness**: Prioritizes long-term stable fixes over quick hacks.
- **Cross-Platform Compatibility**: Targets Windows, macOS, and Linux (including specific hardware driver support like PacDrive).
- **Customizability**: Heavy reliance on user-generated content, NoteSkins, and Lua-based UI Themes (e.g., Simply Love).

## 2. Architecture & Codebase Structure
The engine is primarily written in C++ for performance-critical systems, while the UI, animations, and game rules (scoring/judgments) heavily leverage Lua scripting.

### Directory Layout
- `src/`: Core C++ engine code (Gameplay logic, rendering, input handling, audio syncing, hardware drivers).
- `Themes/`: Lua-based UI Themes (e.g., `_fallback`, `Simply Love`). Controls visual layout, metrics, and historically, scoring logic.
- `extern/`: External C/C++ dependencies tracked as submodules.
- `Docs/`: Project documentation, including known issues, API references, and developer guidelines.
- `CMake/` & `Build/`: CMake configuration files and build scripts.
- `Data/`, `NoteSkins/`, `Songs/`, `Courses/`: Game assets and configurations.

## 3. Dependencies & Submodules
ITGmania utilizes several submodules for essential libraries to ensure consistent cross-platform builds:
- **Audio/Video**: `ffmpeg`, `ogg`, `vorbis`
- **Graphics/Images**: `libjpeg-turbo`, `libpng`
- **Networking/Security**: `IXWebSocket`, `libtomcrypt`, `libtommath`, `mbedtls`
- **Hardware/Input**: `hidapi`, `libusb`
- **Compression**: `zlib`
- **UI/Theming**: `Themes/Simply Love`

## 4. Build System
ITGmania uses **CMake** (v3.10+) as its primary build system.
- **Windows**: Requires DirectX SDK and Windows SDK.
- **Linux**: Requires OpenGL, X11 (`libxinerama-dev`, `libxtst-dev`), ALSA/PulseAudio, GTK3, and LibUSB.
- **macOS**: Requires Xcode Command Line Tools.
The project can be built by running `cmake -DCMAKE_BUILD_TYPE=Release ..` from within the `Build/` directory, followed by `cmake --build .`.

## 5. Current Project State & Known Issues

### State Overview
The project is currently undergoing maintenance to integrate upstream fixes and modernize dependencies. A recent merge operation was performed to pull from upstream, which currently requires manual intervention to correctly resolve 90+ lingering conflict markers in the `src/` directory.

### Known Issues & Backlog
1. **Scoring System Dependency**:
   - *Current State*: Scoring is deeply tied to the Theme's Lua metrics.
   - *Problem*: Themes must explicitly support scoring. This causes bugs like the final note of a song not registering a score, and complicates cross-theme score tracking.
   - *Goal*: Decouple scoring logic from Themes and implement a unified, global C++ scoring system.
2. **Course Editor**:
   - *Current State*: Broken and largely non-functional.
   - *Goal*: Complete architectural rewrite of the Course Editor interface and logic.
3. **LuaDriver Threading**:
   - *Current State*: Lua execution runs synchronously on the main thread, causing frame latency spikes during heavy UI updates.
   - *Goal*: Introduce asynchronous/threaded Lua execution where safe.
4. **Graphics Compatibility**:
   - *Current State*: 3D models can cause crashes on specific graphics cards due to OpenGL `glDrawRangeElements` limitations.
5. **Asset Optimization**:
   - *Current State*: Japanese font maps are too large and slow down debugging builds or downscale poorly.

## 6. Roadmap & Improvement Strategy

### Immediate Action Plan
1. **Code Cleanup**: Safely and manually resolve the remaining 90+ Git conflict markers inside `src/` without corrupting the C++ structure.
2. **Scoring System Refactor**: Move dance points, grade points, and percentage calculation entirely out of `ThemeMetric` configurations to resolve missing final note bugs.
3. **Graphics & Fonts Fixes**: Implement bounds checking for 3D model vertices and slice oversized Japanese font maps.

### Long-term Vision
- Shift towards a fully modern CMake build system, phasing out legacy Visual Studio solution files if they exist.
- Expand continuous integration (CI) to automatically test Lua theme compatibility.
- Further optimize asset loading (threading texture and sound loads) to eliminate micro-stutters.
