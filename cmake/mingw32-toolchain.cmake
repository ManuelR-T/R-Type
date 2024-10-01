set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_VERSION 11)

set(CMAKE_SYSTEM_PROCESSOR x86_64)

set(CMAKE_C_COMPILER /usr/bin/x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER /usr/bin/x86_64-w64-mingw32-g++)

set(CMAKE_RC_COMPILER /usr/bin/x86_64-w64-mingw32-windres)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

add_definitions(-D_WIN32_WINNT=0x0601)
add_definitions(-DWINVER=0x0601)

include_directories(SYSTEM /usr/x86_64-w64-mingw32/include)
link_directories(/usr/x86_64-w64-mingw32/lib)

include_directories(${CMAKE_BINARY_DIR}/_deps/asio-src/asio/include)
