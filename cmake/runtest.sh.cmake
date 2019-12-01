#!/bin/sh

set -e

export LD_LIBRARY_PATH="@CMAKE_BINARY_DIR@/lib"
export QTDIR="@CMAKE_SOURCE_DIR@"
export QT_PLUGIN_PATH="@CMAKE_BINARY_DIR@/plugins"
export QML_IMPORT_PATH="@CMAKE_BINARY_DIR@/imports"

bin="$1"
shift

cd "$(dirname "$bin")"
exec "./$(basename "$bin")" $@
