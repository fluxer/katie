#!/bin/sh

set -e

sofile="$@"
sobase="$(basename $sofile)"
sosym="/tmp/$sobase.sym"
usosym="/tmp/uniq_$sobase.sym"
stripfile="/tmp/stripped_$sobase"
stripsym="/tmp/stripped_$sobase.sym"
ustripsym="/tmp/uniq_stripped_$sobase.sym"

readelf -s -W "$sofile" | awk -F' ' '{print $8}' > "$sosym"

objcopy -v -g --strip-unneeded "$sofile" "$stripfile"
readelf -s -W "$stripfile" | awk -F' ' '{print $8}' > "$stripsym"

uniq "$sosym" "$usosym"
uniq "$stripsym" "$ustripsym"
while read line; do
    grep -q "^${line}$" "$ustripsym" || echo "$line"
done < "$usosym"

rm -vf "$stripfile" "$sosym" "$usosym" "$stripsym" "$ustripsym"
