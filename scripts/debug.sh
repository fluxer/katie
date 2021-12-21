#!/bin/sh

set -e

cwd="$(readlink -f $(dirname $0))"

rm -rf "$cwd/../debug"
mkdir -p "$cwd/../debug"
cd "$cwd/../debug"

export CC=clang-13 CXX=clang++-13
export LSAN_OPTIONS=suppressions="$cwd/../scripts/lsan.supp"
# undefined,float-cast-overflow,thread,memory,address
sanitizers="address"
export CXXFLAGS="$CXXFLAGS -Wall -fsanitize=$sanitizers -fno-omit-frame-pointer -fno-optimize-sibling-calls"
export LDFLAGS="$LDFLAGS -Wall -fsanitize=$sanitizers"

cmake ../ -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_INSTALL_PREFIX=/usr $@
make -j $(nproc || sysctl -n hw.ncpu || echo 1)
