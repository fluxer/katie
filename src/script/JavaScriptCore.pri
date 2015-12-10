# Include file to make it easy to include WebKit into Qt projects

DEFINES += BUILDING_QT__=1
building-libs {
    win32-msvc2008|win32-msvc2010|win32-msvc2012|win32-msvc2013|win32-icc: INCLUDEPATH += $$PWD/JavaScriptCore/os-win32
} else {
    CONFIG(QTDIR_build) {
        QT += webkit
    } else {
        QMAKE_LIBDIR = $$OUTPUT_DIR/lib $$QMAKE_LIBDIR
        QTWEBKITLIBNAME = QtWebKit
        mac:!static:contains(QT_CONFIG, qt_framework):!CONFIG(webkit_no_framework) {
            LIBS += -framework $$QTWEBKITLIBNAME
            QMAKE_FRAMEWORKPATH = $$OUTPUT_DIR/lib $$QMAKE_FRAMEWORKPATH
        } else {
            win32-*|wince* {
                CONFIG(debug, debug|release):build_pass: QTWEBKITLIBNAME = $${QTWEBKITLIBNAME}d
                QTWEBKITLIBNAME = $${QTWEBKITLIBNAME}$${QT_MAJOR_VERSION}
                win32-g++*: LIBS += -l$$QTWEBKITLIBNAME
                else: LIBS += $${QTWEBKITLIBNAME}.lib
            } else {
                LIBS += -lQtWebKit
            }
        }
    }
    DEPENDPATH += $$PWD/WebKit/qt/Api
}
greaterThan(QT_MINOR_VERSION, 5):DEFINES += WTF_USE_ACCELERATED_COMPOSITING

!mac:!unix|symbian {
    DEFINES += USE_SYSTEM_MALLOC
}


BASE_DIR = $$PWD
INCLUDEPATH += $$PWD/WebKit/qt/Api

CONFIG -= warn_on
*-g++*:QMAKE_CXXFLAGS += -Wall -Wreturn-type -fno-strict-aliasing -Wcast-align -Wchar-subscripts -Wformat-security -Wreturn-type -Wno-unused-parameter -Wno-sign-compare -Wno-switch -Wno-switch-enum -Wundef -Wmissing-noreturn -Winit-self

# Enable GNU compiler extensions to the ARM compiler for all Qt ports using RVCT
symbian|*-armcc {
    RVCT_COMMON_CFLAGS = --gnu --diag_suppress 68,111,177,368,830,1293
    RVCT_COMMON_CXXFLAGS = $$RVCT_COMMON_CFLAGS --no_parse_templates
}

symbian|maemo5: DEFINES *= QT_NO_UITOOLS

contains(DEFINES, QT_NO_UITOOLS): CONFIG -= uitools


win32-* {
    LIBS += -lwinmm
}
contains(JAVASCRIPTCORE_JIT,yes) {
    DEFINES+=ENABLE_JIT=1
    DEFINES+=ENABLE_YARR_JIT=1
    DEFINES+=ENABLE_YARR=1
}
contains(JAVASCRIPTCORE_JIT,no) {
    DEFINES+=ENABLE_JIT=0
    DEFINES+=ENABLE_YARR_JIT=0
    DEFINES+=ENABLE_YARR=0
}

# Rules when JIT enabled (not disabled)
!contains(DEFINES, ENABLE_JIT=0) {
    linux*-g++*:greaterThan(QT_GCC_MAJOR_VERSION,3):greaterThan(QT_GCC_MINOR_VERSION,0) {
        QMAKE_CXXFLAGS += -fno-stack-protector
        QMAKE_CFLAGS += -fno-stack-protector
    }
}

wince* {
    INCLUDEPATH += $$QT_SOURCE_TREE/src/3rdparty/ce-compat
    SOURCES += $$QT_SOURCE_TREE/src/3rdparty/ce-compat/ce_time.c
    DEFINES += WINCEBASIC
}

