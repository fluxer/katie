CONFIG += qt warn_off
contains(QT_CONFIG, reduce_exports) {
    CONFIG += hide_symbols
    # workaround for compiler errors on Ubuntu
    linux*-g++*:DEFINES += _GLIBCXX_EXTERN_TEMPLATE=0
}

# the following define could be set globally in case we need it elsewhere
solaris* {
    DEFINES += Q_SOLARIS_VERSION=$$system(uname -r | sed -e 's/5\\.//')
}
