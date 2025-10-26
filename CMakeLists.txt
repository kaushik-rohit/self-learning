cmake_minimum_required(VERSION 3.20)
project(image_processing LANGUAGES CXX)

# ---- Config ----
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Project layout (adjust if yours differs)
set(PROJECT_SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")
set(PROJECT_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")

add_executable(image_app
    "${PROJECT_SOURCE_DIR}/main.cpp"
)

target_include_directories(image_app PRIVATE
    "${PROJECT_INCLUDE_DIR}"
)