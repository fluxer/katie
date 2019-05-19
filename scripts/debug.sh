#!/bin/sh

set -e

cwd="$(dirname $0)"

rm -rf "$cwd/../debug"
mkdir -p "$cwd/../debug"
cd "$cwd/../debug"

export CC=clang CXX=clang++
# undefined,float-cast-overflow,thread,memory,address
sanitizers="address"
export CXXFLAGS="$CXXFLAGS -fsanitize=$sanitizers -fno-omit-frame-pointer -fno-optimize-sibling-calls"
export LDFLAGS="$LDFLAGS -fsanitize=$sanitizers"

cmake ../ -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_INSTALL_PREFIX=/usr \
    -DKATIE_SETTINGS_FULL=/etc/xdg \
    -DKATIE_LDCONF_FULL=/etc/ld.so.conf.d \
    -DKATIE_PROFILE_FULL=/etc/profile.d $@
make -j$(nproc || echo 1)
