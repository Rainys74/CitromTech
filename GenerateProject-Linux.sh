#!/bin/bash

echo "Select a toolchain to generate the project:"
echo
echo "[1] gmake2"
echo "[2] Exit"
echo

read -p "Enter your choice (default is 1): " CHOICE

# Default to 1 if nothing is entered
if [ -z "$CHOICE" ]; then
    CHOICE=1
fi

if [ "$CHOICE" == "1" ]; then
    tools/premake/Linux/premake5 --cc=clang --file=premake5.lua gmake2
else
    echo "Exiting..."
    exit 0
fi
