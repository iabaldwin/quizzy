project(matrix)

add_library(${PROJECT_NAME} 
    Matrix.cpp
)
add_library(Matrix::Matrix ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} 
    PUBLIC 
        ${PROJECT_SOURCE_DIR}
)

set_target_properties(matrix
    PROPERTIES 
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED YES
)

#Testing
include(GoogleTest)
add_executable(matrix_test ${PROJECT_SOURCE_DIR}/test/MatrixTest.cpp)
target_link_libraries(matrix_test 
    PRIVATE 
        matrix 
        CONAN_PKG::gtest
)
gtest_discover_tests(matrix_test)