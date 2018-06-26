project(strings)

cmake_minimum_required (VERSION 3.5)

add_definitions(-std=c++14)

set(sources src/strings.cc)

add_executable(strings ${sources})

install(TARGETS strings DESTINATION bin)
