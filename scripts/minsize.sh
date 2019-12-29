#!/bin/sh

set -e

cwd="$(dirname $0)"

rm -rf "$cwd/../minsize"
mkdir -p "$cwd/../minsize"
cd "$cwd/../minsize"

export CFLAGS="$CFLAGS -flto -fno-exceptions" CXXFLAGS="$CXXFLAGS -flto -fno-exceptions"

cmake ../ -DCMAKE_BUILD_TYPE=MinSizeRel \
    -DCMAKE_INSTALL_PREFIX=/usr $@
make -j$(nproc || echo 1)
