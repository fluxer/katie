#!/bin/sh

set -e

cwd="$(realpath $(dirname $0))"
cmake="cmake-2.8.12.2-Linux-i386"

cd "$cwd"
if which curl; then
    curl -C - -L "https://cmake.org/files/v2.8/$cmake.tar.gz" -o "$cmake.tar.gz"
else
    wget -c "https://cmake.org/files/v2.8/$cmake.tar.gz"
fi

rm -rf "$cwd/../oldmake"
mkdir -p "$cwd/../oldmake"
cd "$cwd/../oldmake"

tar -xaf "$cwd/$cmake.tar.gz"
"$cmake/bin/cmake" ../ -DCMAKE_BUILD_TYPE=Release -DENABLE_TESTING=TRUE -DKATIE_UTILS=ON $@
rm -rf *

tar -xaf "$cwd/$cmake.tar.gz"
"$cmake/bin/cmake" ../ -DCMAKE_BUILD_TYPE=Release -DENABLE_TESTING=TRUE -DKATIE_UTILS=ON -DKATIE_ALLINONE=ON $@
# not making on purpose
