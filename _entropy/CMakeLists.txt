project(entropy)

cmake_minimum_required (VERSION 3.5)

add_definitions(-std=c++14)

set(sources src/entropy.cc)

add_executable(entropy ${sources})

install(TARGETS entropy DESTINATION bin)
