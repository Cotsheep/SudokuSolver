rem All contents in the build and bin directories will be cleared (subdirectories and their contents will be recursively deleted)
rem Please ensure that files in the build or bin subdirectories are automatically generated to avoid mistakenly deleting files created by users
@echo off
pushd .

if not exist build (
    mkdir build
)

pushd .
cd build
del /f /s /q *.*
for /d %%p in (*) do rmdir "%%p" /s /q
popd


if not exist bin (
    mkdir bin
)
cd bin
del /f /s /q *.*
for /d %%p in (*) do rmdir "%%p" /s /q

popd