#!/bin/bash

echo "Select a toolchain to generate the project:"
echo
echo "[1] gmake2"
echo "[2] xcode4"
echo "[3] Exit"
echo

echo -n "Press [1], [2], or [3] to choose (default is 1): "
read -n 1 CHOICE
echo  # to move to the next line

# Default to 1 if nothing is entered
if [ -z "$CHOICE" ]; then
    CHOICE=1
fi

if [ "$CHOICE" == "1" ]; then
    tools/premake/MacOS/premake5 --cc=clang --file=premake5.lua gmake2
elif [ "$CHOICE" == "2" ]; then
    tools/premake/MacOS/premake5 --file=premake5.lua xcode4
elif [ "$CHOICE" == "3" ]; then
    echo "Exiting..."
    exit 0
else
    echo "Invalid choice. Exiting..."
    exit 1
fi
