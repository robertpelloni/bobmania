ITGmania
========

ITGmania is a fork of [StepMania 5.1](https://github.com/stepmania/stepmania/tree/5_1-new), an advanced cross-platform rhythm game for home and arcade use.

[![Continuous integration](https://github.com/itgmania/itgmania/actions/workflows/ci.yml/badge.svg?branch=beta)](https://github.com/itgmania/itgmania/actions/workflows/ci.yml) [![Nightly release](https://github.com/itgmania/itgmania/actions/workflows/release.yml/badge.svg?branch=beta&event=push)](https://github.com/itgmania/itgmania/actions/workflows/release.yml?query=branch%3Abeta+event%3Apush)

## Changes to StepMania 5.1

- Built-in network functionality
- Fully 64-bit, optimized for modern OSes
- Reload new songs from within the song select screen
- The mine fix applied (courtesy of [DinsFire64](https://gist.github.com/DinsFire64/4a3f763cd3033afd55a176980b32a3b5))
- Held misses tracked in the engine for pad debugging
- Fixed overlapping hold bug
- Per-player visual delay
- Per-player disabling of timing windows
- New preference to control note render ordering
- Increased the Stats.xml file size limit to 100MB
- Changed the default binding for P2/back from hyphen to backslash

## Installation

You can choose between using the installer or using the portable build. Using the installer is recommended, because it makes upgrading to new versions easier.

### Windows

**Windows 7 is the minimum supported version.**

 * You will likely have to manually allow the installer to start.

### macOS

**macOS users need to have macOS 11 (Big Sur) or higher to run ITGmania.**
* Move ITGmania.app to the Applications folder, and then run the following command in Terminal:

   * `xattr -c /Applications/ITGmania.app`

* All game data will be located in the user's _Application Support_ folder (`~/Library/Application Support/ITGmania`).
* If you are upgrading from ITGmania 1.1.0 or older, please note all user data will now be located within the _Application Support_ folder.
* macOS users no longer need to add the game to "Input Monitoring" - the keyboard works as-is.
* Please note for macOS users the Edit Mode zoom in/out commands have been changed to Option+Up/Down, to prevent an overlap with a Mission Control shortcut (Ctrl+Up)
 
### Linux

**Linux users should receive all they need from the package manager of their choice.**

* **Debian-based** (Ubuntu, Mint, MX Linux, Pop!_OS, etc):

  * `sudo apt install libgdk-pixbuf-2.0-0 libgl1 libglvnd0 libgtk-3-0 libusb-0.1-4 libxinerama1 libxtst6`

* **Fedora-based** (Bazzite, Nobara, AlmaLinux, etc):

  * `sudo dnf install gdk-pixbuf2 gtk3 libusb-compat-0.1 libXinerama libXtst`

*  **Arch-based** (CachyOS, EndeavourOS, Manjaro, Garuda, etc):

   * `sudo pacman -S mesa gtk3 libusb-compat libxinerama libxtst llvm-libs`

* **Gentoo Linux**:

   * `sudo emerge --ask dev-build/cmake media-libs/alsa-lib media-libs/glew media-libs/libglvnd dev-libs/libusb dev-lang/nasm media-libs/libpulse x11-libs/gtk+ media-sound/alsa-utils`  

* **OpenSUSE Linux**:

   * All editions of OpenSUSE already have everything you need pre-installed.


### Build From Source

ITGmania uses [CMake](http://www.cmake.org/) for its build system, supporting cross-platform compilation.

#### Prerequisites
- **Compiler**: GCC, Clang, or MSVC (C++17 support required).
- **Build System**: CMake 3.10+
- **Dependencies**:
  - **Windows**: DirectX SDK, Windows SDK.
  - **Linux**: OpenGL, X11 (`libxinerama-dev`, `libxtst-dev`), ALSA/PulseAudio, GTK3, LibUSB.
  - **macOS**: Xcode Command Line Tools.

#### Compilation Steps
1. **Clone the repository** with submodules:
   ```bash
   git clone --recursive https://github.com/itgmania/itgmania.git
   cd itgmania
   ```
2. **Generate build files** using CMake:
   ```bash
   mkdir Build
   cd Build
   cmake -DCMAKE_BUILD_TYPE=Release ..
   ```
3. **Compile the project**:
   ```bash
   cmake --build . -j$(nproc)
   ```

#### Technical Specifications
- **Architecture**: 64-bit exclusively (x86_64 / arm64).
- **Graphics API**: OpenGL 2.1+ / DirectX 9 (legacy wrapper) / GLES 2.0 (experimental).
- **Audio API**: ALSA, PulseAudio, OSS (Linux), CoreAudio (macOS), DirectSound/WaveOut (Windows).
- **Scripting**: Embedded Lua 5.1 runtime for UI Theming and input event hooks.
- **Hardware Integration**: Includes native support for specific arcade I/O hardware (e.g., Linux PacDrive, PIUIO) directly in the engine via `libusb` and `hidapi`.


## Resources

* [ITGmania Website](https://www.itgmania.com/)
* [StepMania 5.1 to ITGmania Migration Guide](Docs/Userdocs/sm5_migration.md)
* [Lua for ITGmania](https://quietly-turning.github.io/Lua-For-SM5/LuaAPI?engine=ITGmania)
* Lua API Documentation can be found in the Docs folder.

## Licensing Terms

ITGmania, as well as the [Simply Love](https://github.com/Simply-Love/Simply-Love-SM5) theme, are both under the GPLv3 license, or at your option, any later version.

If ITGmania code is used in your project, we would also appreciate it if you link back to [ITGmania](https://github.com/itgmania/itgmania) as well as [StepMania](https://github.com/stepmania/stepmania).

For specific information/legalese:

* All of our source code is under the [GPLv3 license](https://www.gnu.org/licenses/gpl-3.0.en.html).
* Songs included within the 'StepMania 5' folder are under the [<abbr title="Creative Commons Non-Commercial">CC-NC</abbr> license](https://creativecommons.org/).
* Simply Love is licensed under the GPLv3, or, at your option, any later version.
* The copyright for songs in the 'Club Fantastic' folders rests with the original authors. The content is explicitly NOT placed under a Creative Commons license (or similar license), but has been provided free of charge, for personal or public use, including online broadcasting, tournaments, and other purposes. Go to the [Club Fantastic](https://www.clubfantastic.com/) website for more information.
* The [MAD library](http://www.underbit.com/products/mad/) and [FFmpeg codecs](https://www.ffmpeg.org/) when built with our code use the [GPL license](http://www.gnu.org).

## Credits

### ITGmania Team
- Martin Natano (natano)
- teejusb

### Contributors
- [Club Fantastic](https://wiki.clubfantastic.dance/en/Credits)
- [DinsFire64](https://gist.github.com/DinsFire64/4a3f763cd3033afd55a176980b32a3b5) (Mine Fix)
- [EvocaitArt](https://twitter.com/EvocaitArt) (Enchantment NoteSkin)
- [jenx](https://www.amarion.net/) (Fast Profile Switching)
- [LightningXCE](https://twitter.com/lightningxce) (Cyber NoteSkin)
- [MegaSphere](https://github.com/Pete-Lawrence/Peters-Noteskins) (Note/Rainbow/Vivid NoteSkins)
- [StepMania 5](Docs/credits_SM5.txt)
- [Old StepMania Team](Docs/credits_old_Stepmania_Team.txt)
