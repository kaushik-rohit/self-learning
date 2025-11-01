cmake_minimum_required(VERSION 3.20)
project(image_processing LANGUAGES CXX)

# ---- Config ----
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Project layout
set(PROJECT_SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")
set(PROJECT_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")

# ------------------------------------------------------------------
# Gather source files
# ------------------------------------------------------------------
set(SOURCES
    "${PROJECT_SOURCE_DIR}/main_tsq.cpp"
    "${PROJECT_SOURCE_DIR}/thread_safe_queue.cpp"   # implementation
)

# ------------------------------------------------------------------
# Executable
# ------------------------------------------------------------------
# NOTE: executable name can be anything – here we keep "thread_safe_queue"
add_executable(thread_safe_queue ${SOURCES})

# Include directory for headers
target_include_directories(thread_safe_queue PRIVATE
    "${PROJECT_INCLUDE_DIR}"
)

# Link pthread (required for <thread>)
target_link_libraries(thread_safe_queue PRIVATE pthread)