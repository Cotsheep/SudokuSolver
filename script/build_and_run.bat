@echo off
echo Starting build process...
call .\script\build.bat
if %errorlevel% neq 0 (
    echo Build failed! Exiting...
    exit /b %errorlevel%
)

echo Build succeeded. Starting the program...
call .\script\run.bat
if %errorlevel% neq 0 (
    echo Program execution failed! Exiting...
    exit /b %errorlevel%
)