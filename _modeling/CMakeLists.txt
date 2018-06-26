project(modeling)

cmake_minimum_required (VERSION 3.5)

add_definitions(-std=c++14)

set(sources src/modeling.cc)

add_executable(modeling ${sources})

install(TARGETS modeling DESTINATION bin)
