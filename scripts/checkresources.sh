#!/bin/bash

set -e

modify="no"
for i in $@;do
    case "$i" in
        --modify|-m) modify="yes"
    esac
done

builddir="$(dirname "$0")/../build"
autoinc="-I$(pwd)"
for d in $(find $(pwd) $builddir/include $builddir/privateinclude -type d);do
    autoinc+=" -I$d"
done

allfiles="$(find -iname '*.cpp' -printf '%P\n')"
allfiles+=" $(find -iname '*.h' -printf '%P\n')"
for i in $allfiles;do
    if [ -n "$(moc -nw $i $autoinc)" ];then
        found="no"
        for f in $i ${i//_p_p.h/.cpp} ${i//_p.h/.cpp} ${i/.h/.cpp} ${i//.cpp/.h};do
            # echo " >> Checking: $f"
            if [ -e "$f" ];then
                if grep -q "moc_$(basename $i)" $f ;then
                    found="yes"
                    break
                fi
            fi
        done
        norm="${i//_p_p.h/.cpp}"
        norm="${norm//_p.h/.cpp}"
        norm="${norm//.h/.cpp}"
        if [ "$found" = "no" ] && [ "$modify" = "no" ];then
            echo "moc_$(basename $i) in $norm"
        elif [ "$found" = "no" ] && [ "$modify" = "yes" ];then
            echo "#include \"moc_$(basename $i)\"" >> "$norm"
        fi
    fi
done
