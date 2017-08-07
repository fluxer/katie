# This file is a dull UseQt4 CMake file replacement, it is separate on purpose since it is ment to
# be included from the build system of the project using it and populates the current scope with
# header paths and definitions that may otherwise not be wanted. It is also ment to be included
# after KatieConfig.cmake which defines KATIE_DEFINITIONS and KATIE_INCLUDES. End of drama.

if(NOT KATIE_COMPAT EQUAL FALSE AND NOT KATIE_COMPAT EQUAL OFF)
    add_definitions(${KATIE_DEFINITIONS})
    include_directories(${KATIE_INCLUDES})
endif()