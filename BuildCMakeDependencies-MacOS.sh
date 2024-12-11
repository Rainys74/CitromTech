#!/bin/bash

echo "Select a toolchain to generate the project:"
echo
echo "[1] Debug"
echo "[2] Release"
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
    tools/premake/MacOS/premake5 gmake2 --buildcmake=true --config=Debug
elif [ "$CHOICE" == "2" ]; then
    tools/premake/MacOS/premake5 gmake2 --buildcmake=true --config=Release
elif [ "$CHOICE" == "3" ]; then
    echo "Exiting..."
    exit 0
else
    echo "Invalid choice. Exiting..."
    exit 1
fi
