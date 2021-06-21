# Copyright (C) 2015, Ivailo Monev, <xakepa10@gmail.com>
# Redistribution and use is allowed according to the terms of the BSD license.

# UNIFDEF_EXECUTABLE, HEADERS_DIRECTORY and HEADERS_DEFINITIONS origin from the macro call

file(GLOB headers "$ENV{DESTDIR}/${HEADERS_DIRECTORY}/*.h")

set(bootstrap
    -UQT_BOOTSTRAPPED
    -UQT_MOC
)

set(notsupported
    -UQT_NO_QOBJECT
    -UQT_NO_COMPRESS
    -UQT_NO_THREAD
    -UQT_NO_PROCESS
    -UQT_NO_DATASTREAM
    -UQT_NO_TEXTSTREAM
    -UQT_NO_REGEXP
    -UQT_NO_REGEXP_ANCHOR_ALT
    -UQT_NO_REGEXP_BACKREF
    -UQT_NO_REGEXP_CAPTURE
    -UQT_NO_REGEXP_CCLASS
    -UQT_NO_REGEXP_ESCAPE
    -UQT_NO_REGEXP_INTERVAL
    -UQT_NO_REGEXP_LOOKAHEAD
    -UQT_NO_REGEXP_OPTIM
    -UQT_NO_REGEXP_WILDCARD
    -UQT_NO_STANDARDPATHS
    -UQT_NO_SETTINGS
    -UQT_NO_TEXTCODEC
)

message(STATUS "Optimizing header in: $ENV{DESTDIR}/${HEADERS_DIRECTORY}")
execute_process(
    COMMAND ${UNIFDEF_EXECUTABLE} -m ${bootstrap} ${notsupported} ${HEADERS_DEFINITIONS} ${headers}
    RESULT_VARIABLE unifdef_result
    ERROR_VARIABLE unifdef_output
    OUTPUT_VARIABLE unifdef_output
)
# unifdef exits with status 1 if the output differes, see its man page
if(unifdef_result GREATER 1)
    message(FATAL_ERROR "${unifdef_output} (${unifdef_result})")
endif()
