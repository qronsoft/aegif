@echo off
cmake -B "build" -G "Visual Studio 15 2017 Win64" -DCMAKE_SYSTEM_VERSION:STRING=10.0.17763.0
pause
