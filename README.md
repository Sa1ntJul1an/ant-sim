# Ant Sim
Ant simulation, C++ and SFML

This project is intended to simulate the auto-balancing behavior of different "jobs" in a colony of ants.  Each ant assumes a specific role, and this role determines the actions that the ant takes.  Each ant releases a chemical messenger to signal its current job to other ants.  As ants encounter other ants, they detect the job that each encountered ant is performing and may change jobs if they determine based off of their previous encounters that a specific job is under-represented in the colony. 

compiler link for SFML: 
https://github.com/brechtsanders/winlibs_mingw/releases/download/13.1.0-16.0.5-11.0.0-msvcrt-r5/winlibs-x86_64-posix-seh-gcc-13.1.0-mingw-w64msvcrt-11.0.0-r5.7z
