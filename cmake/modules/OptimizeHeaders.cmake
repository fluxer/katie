# Copyright (C) 2015, Ivailo Monev, <xakepa10@gmail.com>
# Redistribution and use is allowed according to the terms of the BSD license.

# UNIFDEF_EXECUTABLE and HEADERS_DIRECTORY origin from the macro call

set(headers_directory "$ENV{DESTDIR}/${HEADERS_DIRECTORY}")
file(GLOB_RECURSE headers "${headers_directory}/*.h")

set(unifdef_arguments
    # bootstrap
    -UQT_BOOTSTRAPPED
    -UQT_MOC
    # not supported
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
    # exports
    -UKtCore_EXPORTS
    -UKtGui_EXPORTS
    -UKtNetwork_EXPORTS
    -UKtSvg_EXPORTS
    -UKtTest_EXPORTS
    -UKtDeclarative_EXPORTS
    -UKtXml_EXPORTS
    -UKtScript_EXPORTS
    -UKtScriptTools_EXPORTS
    -UKtDBus_EXPORTS
    -UKtUiTools_EXPORTS
    -UKtDesigner_EXPORTS
)

message(STATUS "Optimizing header in: ${headers_directory}")
execute_process(
    COMMAND ${UNIFDEF_EXECUTABLE} -m ${unifdef_arguments} ${headers}
    RESULT_VARIABLE unifdef_result
    ERROR_VARIABLE unifdef_output
    OUTPUT_VARIABLE unifdef_output
)
# unifdef exits with status 1 if the output differes, see its man page
if(unifdef_result GREATER 1)
    message(FATAL_ERROR "${unifdef_output} (${unifdef_result})")
endif()
