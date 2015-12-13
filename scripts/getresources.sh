#!/bin/bash

set -e

builddir="$(dirname "$0")/../build"
autoinc="-I$(pwd)"
for d in $(find $(pwd) $builddir/include $builddir/privateinclude -type d);do
    autoinc+=" -I$d"
done

allfiles="$(find -iname '*.cpp' -printf '%P\n')"
allfiles+="$(find -iname '*.h' -printf '%P\n')"
for i in $allfiles;do
    if [ -n "$(moc -nw $i $autoinc)" ];then
        echo '    ${CMAKE_CURRENT_SOURCE_DIR}/'$i
    fi
done
