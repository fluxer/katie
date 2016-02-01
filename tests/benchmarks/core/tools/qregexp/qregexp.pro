include( $${QT_SOURCE_TREE}/src/3rdparty/webkit/Source/JavaScriptCore/JavaScriptCore.pri )

exists( /usr/include/boost/regex.hpp ){
DEFINES+=HAVE_BOOST
LIBS+=-lboost_regex
}

