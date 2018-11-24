cmake_minimum_required(VERSION 3.8)
project(quizzy)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

# Enable testing for libs
enable_testing()

# Adding other CMakeLists.txt
add_subdirectory(matrix_lib)

# Adding executables
add_executable(entropy entropy.cc)
add_executable(modeling modeling.cc)
add_executable(matching matching.cc)
add_executable(strings strings.cc)

# Linking libraries
target_link_libraries(modeling PUBLIC Matrix::Matrix)

# Defining target properties
set_target_properties(entropy modeling matching strings
    PROPERTIES 
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED YES
)