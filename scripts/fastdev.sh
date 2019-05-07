#!/bin/sh

set -e

cwd="$(dirname $0)"

rm -rf "$cwd/../fastdev"
mkdir -p "$cwd/../fastdev"
cd "$cwd/../fastdev"

export CFLAGS="$CFLAGS -O0 -Wall"
export CXXFLAGS="$CXXFLAGS -O0 -Wall"

cmake ../ -DCMAKE_BUILD_TYPE=FastDev \
    -DCMAKE_INSTALL_PREFIX=/usr \
    -DKATIE_SETTINGS_FULL=/etc/xdg \
    -DKATIE_LDCONF_FULL=/etc/ld.so.conf.d \
    -DKATIE_PROFILE_FULL=/etc/profile.d $@
make -j$(nproc || echo 1)
