# Unified StepMania Deployment & Build Guide

## Overview

The build system for Unified StepMania heavily relies on CMake to manage its diverse set of external libraries (bobcoin, ffmpeg, libpng) and submodules.

## Dependencies (Linux/Debian)

Before configuring, ensure the following packages are installed:

```bash
sudo apt-get update
sudo apt-get install cmake gcc g++ libgl1-mesa-dev libglu1-mesa-dev \
    libx11-dev libxrandr-dev zlib1g-dev libasound2-dev libpulse-dev \
    libogg-dev libvorbis-dev libmad0-dev ffmpeg libavcodec-dev \
    libavformat-dev libswscale-dev
```

## Configuring the Build (CMake)

1.  **Clone the repository and submodules:**
    ```bash
    git clone --recurse-submodules <repository_url>
    cd <repo>
    ```

2.  **Generate Build Files:**
    Create a build directory and invoke CMake. We use the Ninja generator for faster compilation.
    ```bash
    mkdir build
    cd build
    cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ..
    ```

    *Optional Flags:*
    *   `-DWITH_FFMPEG=ON` (Enables the Spectator TV and streaming modules).
    *   `-DWITH_BOBCOIN=ON` (Links the real cryptocurrency library).
    *   `-DWITH_CRASH_HANDLER=OFF` (Disables the legacy crash logger).

3.  **Compile:**
    ```bash
    ninja
    ```

4.  **Run:**
    The executable is placed in the root directory.
    ```bash
    ./stepmania
    ```

## Submodule Strategy

The repository includes submodules like `extern/bobcoin`. If a submodule is out of date:

```bash
git submodule update --remote --merge
git commit -am "Update submodules to latest upstream"
```

## Running the Mock Server

The Tournament and Network modules require a backend server. A Node.js mock server is provided for local testing.

```bash
cd server
npm install ws uuid
node mock_server.js
```

Ensure the server is running on `ws://127.0.0.1:8080` before testing Network modules in-game.
