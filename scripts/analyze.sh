#!/bin/sh

set -e

cwd="$(readlink -f $(dirname $0))"

rm -rf "$cwd/../debug"
mkdir -p "$cwd/../debug"
cd "$cwd/../debug"

scan-build cmake ../ -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_INSTALL_PREFIX=/usr $@
scan-build make -j $(nproc || sysctl -n hw.ncpu || echo 1)
