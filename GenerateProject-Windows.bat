@echo off
echo Select a toolchain to generate the project:
echo.
echo [1] Visual Studio 2022
echo [2] gmake2
echo [3] Custom
echo [4] Exit
echo.

:choice
choice /c 1234 /n /m "Press [1], [2], [3], or [4] to choose (default is 1): "
if errorlevel 4 (
    echo Exiting...
    exit /b
) else if errorlevel 3 (
    :: This weird goto fixes issues...
    goto get_custom_arg
    :get_custom_arg
    set /p CUSTOM_ARG="Enter your custom argument: "

    :: Check if CUSTOM_ARG is empty
    if "%CUSTOM_ARG%"=="" (
        echo Custom argument cannot be empty. Please try again.
        goto get_custom_arg
    )

    call tools\premake\Windows\premake5.exe "%CUSTOM_ARG%"

    goto finish_exit
) else if errorlevel 2 (
    call tools\premake\Windows\premake5.exe gmake2
    goto finish_exit
) else if errorlevel 1 (
    call tools\premake\Windows\premake5.exe vs2022
    goto finish_exit
)

:finish_exit
REM :: echo Done.
PAUSE
exit /b