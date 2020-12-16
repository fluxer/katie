#!/bin/sh

set -e

cwd="$(readlink -f $(dirname $0))"
cmake="cmake-3.3.0-Linux-x86_64.tar.gz"
cmakesite="https://cmake.org/files/v3.3"

cd "$cwd"
if which curl; then
    curl -C - -L "$cmakesite/$cmake.tar.gz" -o "$cmake.tar.gz"
else
    wget -c "$cmakesite/$cmake.tar.gz"
fi

rm -rf "$cwd/../oldmake"
mkdir -p "$cwd/../oldmake"
cd "$cwd/../oldmake"

tar -xaf "$cwd/$cmake.tar.gz"
"$cmake/bin/cmake" ../ -DCMAKE_BUILD_TYPE=Release -DKATIE_TESTS=ON -DKATIE_BENCHMARKS=ON -DKATIE_UTILS=ON $@
rm -rf *

tar -xaf "$cwd/$cmake.tar.gz"
"$cmake/bin/cmake" ../ -DCMAKE_BUILD_TYPE=Release -DKATIE_TESTS=ON -DKATIE_BENCHMARKS=ON -DKATIE_UTILS=ON -DKATIE_ALLINONE=ON $@
rm -rf *

tar -xaf "$cwd/$cmake.tar.gz"
"$cmake/bin/cmake" ../ -GNinja -DCMAKE_BUILD_TYPE=Release -DKATIE_TESTS=ON -DKATIE_BENCHMARKS=ON -DKATIE_UTILS=ON $@
ninja -n
rm -rf *

tar -xaf "$cwd/$cmake.tar.gz"
"$cmake/bin/cmake" ../ -GNinja -DCMAKE_BUILD_TYPE=Release -DKATIE_TESTS=ON -DKATIE_BENCHMARKS=ON -DKATIE_UTILS=ON -DKATIE_ALLINONE=ON $@
ninja -n
