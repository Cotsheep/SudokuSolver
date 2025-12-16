rem config cmake first, then copile source code, the exe file will be put under bin directory.
@echo off
pushd .

if not exist build (
     mkdir build
)

cd build
cmake -G"MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ..
mingw32-make.exe

popd