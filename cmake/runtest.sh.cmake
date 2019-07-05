#!/bin/sh

set -e

export LD_LIBRARY_PATH="@CMAKE_BINARY_DIR@/lib"
export QTDIR="@CMAKE_SOURCE_DIR@"
export QT_PLUGIN_PATH="@CMAKE_BINARY_DIR@/plugins"
export QML_IMPORT_PATH="@CMAKE_BINARY_DIR@/imports"

bin="$1"
shift

export CTEST_OUTPUT_ON_FAILURE=1

# 9 minutes timeout for the tests, Travis timeouts on 10min if there is no
# output and I do not want some tests to be skipped because of one test. Like
# the qpainter test - it does a lot of tests and can take more then 10min.
export CTEST_TEST_TIMEOUT=540


cd "$(dirname "$bin")"
exec "./$(basename "$bin")" $@
