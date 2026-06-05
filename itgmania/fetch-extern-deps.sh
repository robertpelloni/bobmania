#!/bin/bash
# Fetch external build dependencies for itgmania
# These were previously git submodules but are now fetched standalone
# to avoid Jules clone failures from stale third-party submodule pointers.
#
# Usage: ./fetch-extern-deps.sh [--depth 1]

DEPTH=""
if [ "$1" = "--depth" ] && [ -n "$2" ]; then
  DEPTH="--depth $2"
elif [ "$1" = "--shallow" ]; then
  DEPTH="--depth 1"
fi

set -e

DEPS="
IXWebSocket|https://github.com/machinezone/IXWebSocket.git|998cf95dc7f8cf182c54634cc1c2787abc6cb6be
ffmpeg|https://github.com/FFmpeg/FFmpeg.git|b3552002637ab5546dc5df294810d3981465af86
hidapi|https://github.com/libusb/hidapi.git|c3509c11174fe80ff59a47119433a7db5299af85
libjpeg-turbo|https://github.com/libjpeg-turbo/libjpeg-turbo.git|94d5ff43ca91e7e69c984fed0ec0141b5fbd19b3
libpng|https://github.com/pnggroup/libpng.git|92c853c34e41570504baa48b8a8169e53b188324
libtomcrypt|https://github.com/libtom/libtomcrypt.git|a68fa19bc2b532f66a6f18ca457daec53054a312
libtommath|https://github.com/libtom/libtommath.git|ae40a87a920099a7d9d00979570e0c8d917a1fd7
libusb|https://github.com/libusb/libusb-cmake.git|c8477c10ac2ac6b1718d4d498e102b9f18b776f5
mbedtls|https://github.com/Mbed-TLS/mbedtls.git|545d1b77a29ac33b219a6681489d5e63b63c3b3a
ogg|https://github.com/xiph/ogg.git|06a5e0262cdc28aa4ae6797627a783b5010440f0
vorbis|https://github.com/xiph/vorbis.git|1c5f57a2c42ddac91e5c5bb70eb4c56099d442a9
zlib|https://github.com/madler/zlib.git|e3dc0a85b7032e98380dec011bc8f2c2ee0d8fca
"

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
EXTERN_DIR="$SCRIPT_DIR/extern"

echo "Fetching itgmania external dependencies into $EXTERN_DIR"
mkdir -p "$EXTERN_DIR"

echo "$DEPS" | while IFS='|' read -r name url hash; do
  name=$(echo "$name" | xargs)
  url=$(echo "$url" | xargs)
  hash=$(echo "$hash" | xargs)
  [ -z "$name" ] && continue
  
  dest="$EXTERN_DIR/$name"
  if [ -d "$dest" ]; then
    echo "  SKIP $name (already exists)"
    continue
  fi
  
  echo "  FETCH $name @ ${hash:0:12} from $url"
  git clone $DEPTH "$url" "$dest" 2>/dev/null
  if [ -n "$hash" ] && [ -n "$DEPTH" ]; then
    cd "$dest"
    git fetch origin "$hash" 2>/dev/null || true
    git checkout "$hash" 2>/dev/null || true
    cd "$SCRIPT_DIR"
  elif [ -n "$hash" ]; then
    cd "$dest"
    git checkout "$hash" 2>/dev/null || true
    cd "$SCRIPT_DIR"
  fi
done

echo "Done. Run CMake build as usual."
