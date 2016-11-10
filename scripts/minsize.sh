#!/bin/sh

set -e

cwd="$(dirname $0)"

mkdir -p "$cwd/../minsize"
cd "$cwd/../minsize"

export CFLAGS="$CFLAGS -flto" CXXFLAGS="$CXXFLAGS -flto"

cmake ../ -DCMAKE_BUILD_TYPE=MinSizeRel -DCMAKE_INSTALL_PREFIX=/usr $@
make -j$(nproc)
