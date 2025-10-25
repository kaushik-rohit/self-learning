cmake_minimum_required(VERSION 3.16)
project(k-means CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Library
add_library(kmeans
    src/kmeans.cpp
    src/timer.cpp
)

target_include_directories(kmeans PUBLIC include)

add_executable(kmeans_app src/main.cpp)

target_link_libraries(kmeans_app PRIVATE kmeans)
# Tests
enable_testing()
add_executable(test_kmeans tests/test_kmeans.cpp)

target_link_libraries(test_kmeans PRIVATE kmeans)
add_test(NAME unit_tests COMMAND test_kmeans)