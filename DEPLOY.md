# Deployment & Environment Setup

## Build Requirements
- **CMake 3.10+**
- **C++17 Compatible Compiler** (GCC 8+, Clang 7+, MSVC 2019+)
- **Dependencies:** FFmpeg, OpenSSL, libusb, GTK3 (Linux), DirectX SDK (Windows).

## Compilation Instructions
### Linux
```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

### Windows
Use `CMake` to generate a Visual Studio solution, then build the `StepMania` project.

## Running the Server (Bobcoin & Matchmaking)
The backend is a Node.js application located in `server/`.
```bash
cd server
npm install
npm start
```

## Bobcoin Node
To use real economy features, you must have a `bobcoin` node running locally or reachable via RPC.
Update `Data/Static.ini` with your node's IP and port.
