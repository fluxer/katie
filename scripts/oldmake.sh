#!/bin/sh

set -e

cwd="$(realpath $(dirname $0))"

cd "$cwd"
if which wget; then
    wget -c https://cmake.org/files/v2.8/cmake-2.8.12.2-Linux-i386.tar.gz
else
    curl -C - https://cmake.org/files/v2.8/cmake-2.8.12.2-Linux-i386.tar.gz -o cmake-2.8.12.2-Linux-i386.tar.gz
fi

rm -rf "$cwd/../oldmake"
mkdir -p "$cwd/../oldmake"
cd "$cwd/../oldmake"

tar -xaf "$cwd/cmake-2.8.12.2-Linux-i386.tar.gz"

cmake-2.8.12.2-Linux-i386/bin/cmake ../ -DCMAKE_BUILD_TYPE=FastDev -DKATIE_BINDINGS=ON $@
make -j$(nproc || echo 1)
