project(matching)

cmake_minimum_required (VERSION 3.5)

add_definitions(-std=c++14)

set(sources src/matching.cc)

add_executable(matching ${sources})

install(TARGETS matching DESTINATION bin)
