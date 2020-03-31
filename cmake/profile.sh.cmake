#!@KATIE_SHELL@

# NOTE: safe as in does not append if already in the variable
safe_path_append() {
    # if the environment variable is empty just export
    if [ -z "$2" ];then
        export "$1"="$3"
        return
    fi

    # else append to it if neccessary
    noslash="$(echo "$2" | sed 's|/$||')"
    case "$2" in
        "$3"|*:"$3"|"$3":*|*:"$3":*) ;;
        "$3/"|*:"$3/"|"$3/":*|*:"$3/":*) ;;
        "$noslash"|*:"$noslash"|"$noslash":*|*:"$noslash":*) ;;
        *) export "$1"="$2:$3" ;;
    esac
}

safe_path_append PATH "$PATH" "@KATIE_BINARIES_PATH@"

safe_path_append LD_LIBRARY_PATH "$LD_LIBRARY_PATH" "@KATIE_LIBRARIES_PATH@"

safe_path_append PKG_CONFIG_PATH "$PKG_CONFIG_PATH" "@KATIE_PKGCONFIG_PATH@"

export Katie_DIR="@KATIE_CMAKE_PATH@"

unset -f safe_path_append
