@echo off
echo Building itgmania...
cmake -B build -S .
cmake --build build --config Release
echo Build complete.
pause