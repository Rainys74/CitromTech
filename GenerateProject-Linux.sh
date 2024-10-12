#!/bin/bash

echo "Select a toolchain to generate the project:"
echo
echo "[1] gmake2"
echo "[2] Exit"
echo

echo -n "Press [1] or [2] to choose (default is 1): "
read -n 1 CHOICE
echo  # to move to the next line

# Default to 1 if nothing is entered
if [ -z "$CHOICE" ]; then
    CHOICE=1
fi

if [ "$CHOICE" == "1" ]; then
    tools/premake/Linux/premake5 --cc=clang --file=premake5.lua gmake2
elif [ "$CHOICE" == "2" ]; then
    echo "Exiting..."
    exit 0
else
    echo "Invalid choice. Exiting..."
    exit 1
fi
