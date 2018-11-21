cmake_minimum_required (VERSION 3.1)
project (WoddenCannon)

include(CTest)

set (CMAKE_CXX_STANDARD 11)

if (WIN32)
    add_compile_options( /W4 /D_CRT_SECURE_NO_WARNINGS /wd4100 )
elseif(UNIX)
    add_compile_options( -Wall -Wno-invalid-offsetof)
endif(WIN32)

add_definitions(-DWC_NETWORK -DWC_INITIAL_BUFFER_SIZE=16)

add_subdirectory(test_src)
add_subdirectory(src)
