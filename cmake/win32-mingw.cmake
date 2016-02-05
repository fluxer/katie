# use like this: cmake . -DCMAKE_TOOLCHAIN_FILE=cmake/win32-mingw.cmake

set(mingwroot "C:/MinGW")

# which compilers to use for C and C++
set(CMAKE_C_COMPILER ${mingwroot}/bin/gcc.exe)
set(CMAKE_CXX_COMPILER ${mingwroot}/bin/g++.exe)
set(CMAKE_RC_COMPILER ${mingwroot}/bin/windres.exe)

# here is the target environment located
set(CMAKE_FIND_ROOT_PATH ${mingwroot})

# adjust the default behaviour of the FIND_XXX() commands:
# search headers and libraries in the target environment, search 
# programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
