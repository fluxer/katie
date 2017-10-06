#!/bin/sh

# NOTE: safe as in does not append if already in the variable
safe_path_append() {
    # if the environment variable is empty just export
    if [ -z "$2" ];then
        export "$1"="$3"
        return
    fi

    # else append to it if neccessary
    oldIFS="$IFS"
    IFS=":"
    found=""
    for i in $2;do
        # TODO: maybe check for match without trailing slash too?
        if [ "$i" = "$3" ];then
            found="yes"
        fi
    done
    IFS="$oldIFS"

    if [ "$found" != "yes" ];then
        export "$1"="$2:$3"
    fi
}

safe_path_append PATH "$PATH" "@KATIE_BINARIES_FULL@"

safe_path_append LD_LIBRARY_PATH "$LD_LIBRARY_PATH" "@KATIE_LIBRARIES_FULL@"

safe_path_append PKG_CONFIG_PATH "$PKG_CONFIG_PATH" "@KATIE_PKGCONFIG_FULL@"

export Katie_DIR="@KATIE_CMAKE_FULL@"

unset -f safe_path_append
