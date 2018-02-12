#!/bin/sh

set -e

cwd="$(dirname $0)"

rm -rf "$cwd/../minsize"
mkdir -p "$cwd/../minsize"
cd "$cwd/../minsize"

export CFLAGS="$CFLAGS -flto" CXXFLAGS="$CXXFLAGS -flto"

cmake ../ -DCMAKE_BUILD_TYPE=MinSizeRel $@
make -j$(nproc || echo 1)
