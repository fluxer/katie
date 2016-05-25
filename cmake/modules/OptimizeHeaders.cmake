# UNIFDEF_EXECUTABLE, HEADERS_DIRECTORY and HEADERS_DEFINITIONS origin from the macro call

file(GLOB headers "$ENV{DESTDIR}/${HEADERS_DIRECTORY}/*.h")

message(STATUS "Optimizing header in: $ENV{DESTDIR}/${HEADERS_DIRECTORY}")
execute_process(
    COMMAND ${UNIFDEF_EXECUTABLE} -m -UQT_BOOTSTRAPPED -UQT_MOC -UQT_RCC -UQT_UIC ${HEADERS_DEFINITIONS} ${headers}
    RESULT_VARIABLE unifdef_result
    ERROR_VARIABLE unifdef_output
    OUTPUT_VARIABLE unifdef_output
)
# unifdef exits with status 1 if the output differes, see its man page
if(unifdef_result GREATER 1)
    message(FATAL_ERROR "${unifdef_output} (${unifdef_result})")
endif()
