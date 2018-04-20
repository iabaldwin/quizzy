cmake_minimum_required(VERSION 3.8)
project(quizzy)

set(CMAKE_CXX_STANDARD 14)

set(ENTROPY_SOURCE_FILES entropy.cc)
set(MODELING_SOURCE_FILES modeling.cc Matrix.cpp)
set(MATCHING_SOURCE_FILES matching.cc)
set(STRINGS_SOURCE_FILES strings.cc)
set(MATRIX_SOURCE_FILES Matrix.cpp include/Matrix.h)

add_executable(entropy ${ENTROPY_SOURCE_FILES})
add_executable(modeling ${MODELING_SOURCE_FILES})
add_executable(matching ${MATCHING_SOURCE_FILES})
add_executable(strings ${STRINGS_SOURCE_FILES})

add_library(matrix ${MATRIX_SOURCE_FILES})


#Testing
enable_testing()
find_package(GTest REQUIRED)

add_executable(matrixTest tests/matrixTest.cpp)
target_link_libraries(matrixTest GTest::GTest GTest::Main)
target_link_libraries(matrixTest matrix)
target_include_directories(matrixTest PRIVATE include)
add_test(tests matrixTest)