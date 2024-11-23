@echo off
echo Choose the configuration for the build:
echo.
echo [1] Debug
echo [2] Release
echo [3] Exit
echo.

:choice
choice /c 123 /n /m "Press [1], [2], or [3] to choose (default is 1): "
if errorlevel 3 (
    echo Exiting...
    exit /b
) else if errorlevel 2 (
    call tools\premake\Windows\premake5.exe gmake2 --buildcmake=true --config=Release
    goto finish_exit
) else if errorlevel 1 (
    call tools\premake\Windows\premake5.exe gmake2 --buildcmake=true --config=Debug
    goto finish_exit
)

:finish_exit
REM :: echo Done.
PAUSE
exit /b