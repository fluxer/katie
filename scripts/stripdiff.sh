#!/bin/sh

set -e

sofile="$@"
sobase="$(basename $sofile)"
sosym="/tmp/$sobase.sym"
stripfile="/tmp/stripped_$sobase"
stripsym="/tmp/stripped_$sobase.sym"

readelf -s -W "$sofile" | awk -F' ' '{print $8}' > "$sosym"

objcopy -v -g "$sofile" "$stripfile"
readelf -s -W "$stripfile" | awk -F' ' '{print $8}' > "$stripsym"

diff -u "$sosym" "$stripsym"

rm -vf /tmp/*.so /tmp/*.sym
