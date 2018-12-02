cmake_minimum_required (VERSION 3.1)
project (TestWc)

set (CMAKE_CXX_STANDARD 11)

file(GLOB Sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
file(GLOB Includes RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.h)

add_executable(testwc ${Sources} ${Includes})

target_include_directories(testwc PRIVATE
        ${CMAKE_SOURCE_DIR}/inc
        ${CMAKE_CURRENT_SOURCE_DIR} )

target_link_libraries(testwc wc)

if (WIN32)
    target_link_libraries(testwc ws2_32)
endif(WIN32)

add_dependencies(testwc wc)

add_test(NAME serialize COMMAND testwc -o -n 1000000)
add_test(NAME reconstruct COMMAND testwc)
