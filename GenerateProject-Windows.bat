@echo off
echo Select a toolchain to generate the project:
echo.
echo [1] Visual Studio 2022
echo [2] gmake2
echo [3] Exit
echo.

:choice
set /p CHOICE="Press [1], [2], or [3] to choose (default is 1): "
if "%CHOICE%"=="" set CHOICE=1

if "%CHOICE%"=="1" (
    call tools\premake\Windows\premake5.exe vs2022
) else if "%CHOICE%"=="2" (
    call tools\premake\Windows\premake5.exe gmake2
) else if "%CHOICE%"=="3" (
    echo Exiting...
    exit /b
) else (
    echo Invalid choice. Please try again.
    goto choice
)

PAUSE
