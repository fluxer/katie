#!/bin/sh

set -e

cwd="$(readlink -f $(dirname $0))"

rm -rf "$cwd/../debug"
mkdir -p "$cwd/../debug"
cd "$cwd/../debug"

export CXXFLAGS="$CXXFLAGS -fprofile-arcs -ftest-coverage"
export LDFLAGS="$LDFLAGS -fprofile-arcs -ftest-coverage"

cmake ../ -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_INSTALL_PREFIX=/usr $@ \
    -DKATIE_TESTS=ON
make -j $(nproc || sysctl -n hw.ncpu || echo 1)

gcovr -r ../src $PWD
