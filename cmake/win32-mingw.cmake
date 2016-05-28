# use like this: cmake . -DCMAKE_TOOLCHAIN_FILE=cmake/win32-mingw.cmake

set(mingwroot "C:/MinGW")
set(gnuwin32root "C:/Program Files/GnuWin32")

# which compilers to use for C and C++
set(CMAKE_C_COMPILER ${mingwroot}/bin/gcc.exe)
set(CMAKE_CXX_COMPILER ${mingwroot}/bin/g++.exe)
set(CMAKE_RC_COMPILER ${mingwroot}/bin/windres.exe)

# here is the target environment located
set(CMAKE_FIND_ROOT_PATH "${mingwroot}" "${gnuwin32root}")
# ensure that libraries and headers from MinGW and GnuWin32 can be found
set(CMAKE_LIBRARY_PATH "${CMAKE_LIBRARY_PATH}" "${mingwroot}/bin" "${gnuwin32root}/bin")
set(CMAKE_INCLUDE_PATH "${CMAKE_INCLUDE_PATH}" "${mingwroot}/include" "${gnuwin32root}/include")

# adjust the default behaviour of the FIND_XXX() commands:
# search headers and libraries in the target environment, search 
# programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
