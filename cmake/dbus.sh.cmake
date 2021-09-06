#!@KATIE_SHELL@

set -e

if [ -z "$LD_LIBRARY_PATH" ];then
    export LD_LIBRARY_PATH="@CMAKE_BINARY_DIR@/lib"
else
    export LD_LIBRARY_PATH="@CMAKE_BINARY_DIR@/lib:$LD_LIBRARY_PATH"
fi
export QT_PLUGIN_PATH="@CMAKE_BINARY_DIR@/plugins"
export QML_IMPORT_PATH="@CMAKE_BINARY_DIR@/imports"

bin="$1"
shift

cd "$(dirname "$bin")"
if [ -z "$DBUS_SESSION_BUS_ADDRESS" ];then
    exec dbus-run-session -- @CMAKE_CROSSCOMPILING_EMULATOR@ "./$(basename "$bin")" "$@"
fi
exec @CMAKE_CROSSCOMPILING_EMULATOR@ "./$(basename "$bin")" "$@"
