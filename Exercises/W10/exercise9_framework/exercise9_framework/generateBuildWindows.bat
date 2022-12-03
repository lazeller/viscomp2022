@echo off

set CMAKE_GENERATOR="Visual Studio 17 2022"

mkdir build
cd build

cmake -G"Visual Studio 17 2022" ../

cd ..