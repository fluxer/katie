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
if [ -z "$DISPLAY" ];then
    Xvfb :123 &
    xvfbpid=$!
    sleep 3

    DISPLAY=:123 @KATIE_CROSS_EXEC@ "./$(basename "$bin")" $@
    retval=$?

    kill $xvfbpid
    exit $retval
fi
exec @KATIE_CROSS_EXEC@ "./$(basename "$bin")" $@
