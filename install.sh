#!/bin/bash

rm -rf build
mkdir build

cxx="g++"
build_type="Release"

if [ "$1" == "debug" ]
then
    build_type="Debug"
fi

if [ "$1" == "exp" ]  || [ "$2" == "exp" ]
then
   EXP="-DEXPANSION=ON"
fi

cd build && cmake ../rpl-shell -DCMAKE_CXX_COMPILER=$cxx -DCMAKE_BUILD_TYPE=$build_type $EXP
make
