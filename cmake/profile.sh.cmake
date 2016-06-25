#!/bin/sh

if [ -n "$LD_LIBRARY_PATH" ];then
    export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:@QT_LIBRARIES_PATH@"
else
    export LD_LIBRARY_PATH="@QT_LIBRARIES_PATH@"
fi

if [ -n "$PKG_CONFIG_PATH" ];then
    export PKG_CONFIG_PATH="$PKG_CONFIG_PATH:@QT_LIBRARIES_PATH@/pkgconfig"
else
    export PKG_CONFIG_PATH="@QT_LIBRARIES_PATH@/pkgconfig"
fi

export Katie_DIR="@CMAKE_INSTALL_PATH@"
