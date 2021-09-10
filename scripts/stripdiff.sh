#!/bin/sh

set -e

demanglersrc="/tmp/demangler.cpp"
demanglerfile="/tmp/demangler"
printf "
#include <stdlib.h>
#include <stdio.h>
#include <cxxabi.h>

int main(int argc, char** argv) {
    int status;
    char* demangled = abi::__cxa_demangle(argv[1], nullptr, nullptr, &status);
    if (status == 0) {
        ::printf(\"%%s\\\n\", demangled);
        ::free(demangled);
    }
    return 0;
}
" > "$demanglersrc"
g++ "$demanglersrc" -o "$demanglerfile"

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
    grep -q "^${line}$" "$ustripsym" || "$demanglerfile" "$line"
done < "$usosym"

rm -vf "$demanglersrc" "$demanglerfile" "$stripfile" "$sosym" "$usosym" "$stripsym" "$ustripsym"
