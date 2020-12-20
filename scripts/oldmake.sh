#!/bin/sh

set -e

cwd="$(readlink -f $(dirname $0))"
cmakeext="tar.gz"
cmake="cmake-3.3.0-Linux-x86_64"
cmakesite="https://cmake.org/files/v3.3"

cd "$cwd"
if which curl; then
    curl -C - -L "$cmakesite/$cmake.$cmakeext" -o "$cmake.$cmakeext"
else
    wget -c "$cmakesite/$cmake.$cmakeext"
fi

rm -rf "$cwd/../oldmake"
mkdir -p "$cwd/../oldmake"
cd "$cwd/../oldmake"

tar -xaf "$cwd/$cmake.$cmakeext"
"$cmake/bin/cmake" ../ -DKATIE_TESTS=ON -DKATIE_BENCHMARKS=ON -DKATIE_UTILS=ON $@
rm -rf *

tar -xaf "$cwd/$cmake.$cmakeext"
"$cmake/bin/cmake" ../ -GNinja -DKATIE_TESTS=ON -DKATIE_BENCHMARKS=ON -DKATIE_UTILS=ON $@
ninja -n
rm -rf *
