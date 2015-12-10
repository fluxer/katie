# FROM: https://gist.github.com/ajmontag/4043875
#
# - try to find the dbus library
#
# Finddbus.cmake
#
# Cache Variables: (probably not for direct use in CMakeLists.txt)
#  DBUS_ROOT_DIR
#  DBUS_LIBRARY
#  DBUS_INCLUDE_DIR
#
# Non-cache variables you might use in your CMakeLists.txt:
#  DBUS_FOUND
#
#  DBUS_LIBRARIES
#  DBUS_INCLUDE_DIRS
#
# Use this module this way:
#  find_package(dbus)
#  include_directories(DBUS_INCLUDE_DIRS)
#  add_executable(myapp ${SOURCES})
#  target_link_libraries(myapp ${DBUS_LIBRARIES})
#
# Requires these CMake modules:
#  FindPackageHandleStandardArgs (CMake standard module)
#  FindPkgConfig (CMake standard module)
#
# Dependencies:
#  none
#
# BEGIN_DOT_FILE
#  dbus [ label = "dbus" ];
# END_DOT_FILE
#
# Author:
#   Kevin M. Godby <kevin@godby.org>
#
# License:
#   Boost 1.0 <http://www.boost.org/users/license.html>

set(DBUS_ROOT_DIR
    "${DBUS_ROOT_DIR}"
    CACHE
    PATH
    "Prefix directory for libdbus")

# Now let's find the dbus library
find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
    pkg_check_modules(_dbus_hint QUIET dbus-1)
endif()

set(dbus_vars DBUS_INCLUDE_DIR)
##find_library(DBUS_LIBRARY
##    NAMES
##    dbus
##    HINTS
##    ${DBUS_ROOT_DIR}
##    ${_dbus_hint_LIBRARY_DIRS}
##    PATH_SUFFIXES
##    lib
##    lib32
##    lib64
##)
##list(APPEND dbus_vars DBUS_LIBRARY)

find_path(DBUS_INCLUDE_DIR
    NAMES
    dbus/dbus.h
    HINTS
    ${DBUS_ROOT_DIR}
    ${_dbus_hint_INCLUDE_DIRS}
    PATH_SUFFIXES
    include/
    include/dbus-1.0/
    dbus-1.0/
)

find_path(DBUS_ARCH_INCLUDE_DIR
    NAMES
    dbus/dbus-arch-deps.h
    HINTS
    ${DBUS_ROOT_DIR}
    ${_dbus_hint_INCLUDE_DIRS}
    ${_dbus_hint_LIBRARY_DIRS}
    PATH_SUFFIXES
    include/
    include/dbus-1.0/
    dbus-1.0/
)
list(APPEND dbus_vars DBUS_ARCH_INCLUDE_DIR)

# handle the QUIETLY and REQUIRED arguments and set DBUS_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(dbus
    DEFAULT_MSG
    ${dbus_vars})

if(DBUS_FOUND)
    # Set variables containing libraries and their dependencies
    # Always use the plural form for the variables defined by other find modules:
    # they might have dependencies too!
    ##list(APPEND DBUS_LIBRARIES ${DBUS_LIBRARY})
    set(DBUS_INCLUDE_DIRS ${DBUS_INCLUDE_DIR} ${DBUS_ARCH_INCLUDE_DIR})
    mark_as_advanced(DBUS_ROOT_DIR)
endif()

mark_as_advanced(${dbus_vars})

# End of Finddbus.cmake
