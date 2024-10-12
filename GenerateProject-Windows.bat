@echo off
echo Select a toolchain to generate the project:
echo.
echo [1] Visual Studio 2022
echo [2] gmake2
echo [3] Exit
echo.

set /p CHOICE="Enter your choice (default is 1): "

REM Default to 1 if nothing is entered
if "%CHOICE%"=="" set CHOICE=1

if "%CHOICE%"=="1" (
    call tools\premake\Windows\premake5.exe vs2022
) else if "%CHOICE%"=="2" (
    call tools\premake\Windows\premake5.exe gmake2
) else (
    echo Exiting...
    exit /b
)

PAUSE
