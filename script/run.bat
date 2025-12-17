rem Automatically execute the exe files in the bin directory (if there are multiple exe files, the last found one will be automatically executed).
@echo off
pushd .

if not exist bin (
    echo "bin directory not exists!"
    popd
    exit(1)
)

set EXE_FILE=""
for /r ./bin %%f in (*.exe) do (
    set EXE_FILE=%%f
)
echo %EXE_FILE%

if EXE_FILE!=="" (
    echo "exe file not exists in bin directory!"
    popd
    exit(1)
)

%EXE_FILE%

popd