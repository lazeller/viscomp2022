@echo off

set CMAKE_GENERATOR="Visual Studio 16 2019"

mkdir build
cd build

cmake -G%CMAKE_GENERATOR% ../

cd ..