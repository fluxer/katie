# HEADERS_DIRECTORY and HEADERS_DEFINITIONS origin from the macro call

file(GLOB headers "$ENV{DESTDIR}/${HEADERS_DIRECTORY}/*.h")

message(STATUS "Optimizing header in: $ENV{DESTDIR}/${HEADERS_DIRECTORY}")
foreach(header ${headers})
    execute_process(
        COMMAND unifdef -UQT_BOOTSTRAPPED ${HEADERS_DEFINITIONS} "${header}" -o "${header}"
    )
endforeach()
