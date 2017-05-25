#!/bin/bash

rm -rf build
mkdir build

cxx="g++"
build_type="Release"

if [ "$1" == "debug" ]
then
    build_type="Debug"
fi

cd build && cmake ../rpl-shell -DCMAKE_CXX_COMPILER=$cxx -DCMAKE_BUILD_TYPE=$build_type
make
