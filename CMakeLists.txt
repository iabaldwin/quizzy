cmake_minimum_required(VERSION 3.8)
project(quizzy)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(matrix_lib)

add_executable(entropy entropy.cc)

add_executable(modeling modeling.cc)
target_link_libraries(modeling PUBLIC Matrix::Matrix)

add_executable(matching matching.cc)

add_executable(strings strings.cc)
