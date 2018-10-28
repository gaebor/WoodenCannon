cmake_minimum_required (VERSION 3.1)
project (wc)

set (CMAKE_CXX_STANDARD 11)

file(GLOB Sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
file(GLOB Includes RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.h)
file(GLOB WcIncludes RELATIVE ${CMAKE_SOURCE_DIR}/inc *.h)

add_library(wc ${Sources} ${Includes} ${WcIncludes})

target_include_directories(wc PRIVATE
        ${CMAKE_SOURCE_DIR}/inc
        ${CMAKE_CURRENT_SOURCE_DIR} )
