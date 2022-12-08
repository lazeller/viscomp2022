@echo off

set CMAKE_GENERATOR="Visual Studio 17 2022"

mkdir build
cd build

cmake -G%CMAKE_GENERATOR% ../

cd ..