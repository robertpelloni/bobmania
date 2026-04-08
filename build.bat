@echo off
echo Building bobmania...
cmake -B build -S .
cmake --build build --config Release
echo Build complete.
pause