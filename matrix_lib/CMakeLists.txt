project(matrix)

add_library(${PROJECT_NAME} 
    Matrix.cpp
)
add_library(Matrix::Matrix ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} 
    PUBLIC 
        ${PROJECT_SOURCE_DIR}
)


# #Testing
# enable_testing()
# find_package(GTest REQUIRED)

# add_executable(matrixTest tests/matrixTest.cpp)
# target_link_libraries(matrixTest GTest::GTest GTest::Main)
# target_link_libraries(matrixTest matrix)
# target_include_directories(matrixTest PRIVATE include)
# add_test(tests matrixTest)