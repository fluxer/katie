# HEADERS_DIRECTORY and HEADERS_DEFINITIONS origin from the macro call

file(GLOB headers "$ENV{DESTDIR}/${HEADERS_DIRECTORY}/*.h")

message(STATUS "Optimizing header in: $ENV{DESTDIR}/${HEADERS_DIRECTORY}")
foreach(header ${headers})
    execute_process(
        COMMAND unifdef -UQT_BOOTSTRAPPED -UQT_MOC -UQT_RCC -UQT_UIC ${HEADERS_DEFINITIONS} "${header}" -o "${header}"
        RESULT_VARIABLE unifdef_result
        ERROR_VARIABLE unifdef_error
    )
    if(NOT unifdef_result EQUAL 0)
        message(SEND_ERROR "${unifdef_error}")
    endif()
endforeach()
