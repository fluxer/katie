#!/bin/sh

set -e

cwd="$(dirname $0)"

rm -rf "$cwd/../minsize"
mkdir -p "$cwd/../minsize"
cd "$cwd/../minsize"

# export CFLAGS="$CFLAGS -flto" CXXFLAGS="$CXXFLAGS -flto"

cmake ../ -DCMAKE_BUILD_TYPE=MinSizeRel \
    -DCMAKE_INSTALL_PREFIX=/usr \
    -DKATIE_SETTINGS_FULL=/etc/xdg \
    -DKATIE_LDCONF_FULL=/etc/ld.so.conf.d \
    -DKATIE_PROFILE_FULL=/etc/profile.d $@
make -j$(nproc || echo 1)
