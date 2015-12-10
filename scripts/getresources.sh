#!/bin/bash

set -e

builddir="$(dirname "$0")/../build"
autoinc="-I$(pwd)"
for d in $(find $(pwd) $builddir/include $builddir/privateinclude -type d);do
    autoinc+=" -I$d"
done

for i in $(find -iname '*.cpp' -o -iname '*.h' -printf '%P\n');do
    if [ -n "$(moc -nw $i $autoinc)" ];then
        echo '    ${CMAKE_CURRENT_SOURCE_DIR}/'$i
    fi
done
