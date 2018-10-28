cmake_minimum_required (VERSION 3.1)
project (WoddenCannon)

include(CTest)

if (WIN32)
    add_compile_options( /W4 /D_CRT_SECURE_NO_WARNINGS /DWC_NETWORK)
elseif(UNIX)
    add_compile_options( -Wall -Wno-invalid-offsetof -DWC_NETWORK )
endif(WIN32)

add_subdirectory(test_src)
add_subdirectory(src)
