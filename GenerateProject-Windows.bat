@echo off
echo Select a toolchain to generate the project:
echo.
echo [1] Visual Studio 2022
echo [2] gmake2
echo [3] Exit
echo.

:choice
choice /c 123 /n /m "Press [1], [2], or [3] to choose (default is 1): "
if errorlevel 3 (
    echo Exiting...
    exit /b
) else if errorlevel 2 (
    call tools\premake\Windows\premake5.exe gmake2
) else if errorlevel 1 (
    call tools\premake\Windows\premake5.exe vs2022
)

REM :: echo Done.
PAUSE
