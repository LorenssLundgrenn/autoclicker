This is a simple autoclicker program with unecessary extra features. This project was
written with only the windows OS in mind. It should be possible to build with this command:
cmake -S ./ -B ./build -G "MinGW Makefiles" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++

This program accepts an optional commandline argument for the path of an instructionset 
file to load with: >> clicker.exe <optional_path>

There are no external dependencies.

keybinds:

SHIFT + ESCAPE: Exit program
CTRL + 1: run loaded instructionset (activate this macro again to disable playback)
CTRL + 2: record new instructionset (activate this macro again to disable recording)
CTRL + S: save the current loaded instructionset to file (make a new .bin file)
CTRL + L: load a instructionset from file
CTRL + P: print current instructionset (format: command_type, {pos_x, pos_y}, delay_ms) [DEBUG]
CTRL + R: resets the loaded instructionset to the default hardcoded instructionset

Note: you cannot record and run an instructionset at the same time