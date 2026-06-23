# Unified StepMania Deployment & Build Guide

## Overview
This document contains the latest instructions for compiling the Unified StepMania C++ backend and deploying the associated Node.js matchmaking/relay servers.

## 1. C++ Engine Compilation (Linux/macOS)

### Prerequisites
*   CMake (3.10+)
*   GCC or Clang (C++11 support required)
*   Libraries: X11, ALSA, libmad, libogg, libvorbis, FFmpeg (for StreamManager), libtorrent-rasterbar (for ContentSwarmManager).

### Build Steps
1.  Clone the repository and initialize submodules:
    ```bash
    git clone --recursive <repository-url>
    cd stepmania-unified
    ```
    *(If submodules fail, initialize them manually: `git submodule update --init`)*
2.  Generate Makefiles:
    ```bash
    mkdir build && cd build
    cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ..
    ```
3.  Compile:
    ```bash
    make -j$(nproc)
    ```
4.  Run:
    ```bash
    ./stepmania
    ```

## 2. Server Deployment (Node.js)

The Unified platform relies on a lightweight Node.js backend for WebSockets, chat relay, and tournament matchmaking.

### Prerequisites
*   Node.js (v18+)
*   npm or yarn

### Deployment Steps
1.  Navigate to the server directory:
    ```bash
    cd server
    ```
2.  Install dependencies:
    ```bash
    npm install
    ```
3.  Start the server (Development):
    ```bash
    node mock_server.js
    ```
4.  Start the server (Production via PM2):
    ```bash
    npm install -g pm2
    pm2 start mock_server.js --name "stepmania-unified-backend"
    pm2 save
    ```

## 3. Blockchain Node Integration (Bobcoin)
To fully utilize the `EconomyManager`, a local or remote Bobcoin node must be running.
1. Compile `extern/bobcoin` according to its internal documentation.
2. Start the RPC server.
3. Ensure the RPC credentials and IP address match the settings in `Save/Economy.xml` (or the default fallbacks in `BobcoinBridge.cpp`).

## Automated Deployment Script
For rapid testing during AI agent sessions, a `deploy.sh` script is provided in the root directory. It simulates building the frontend, checking server dependencies, and restarting the backend service.

Run it via:
```bash
bash deploy.sh
```
