cmake_minimum_required(VERSION 3.20)
project(cgame CXX)

cmake_policy(SET CMP0135 NEW)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# MSVC 需要 /utf-8 选项以正确解析中文注释
if(MSVC)
    add_compile_options(/utf-8)
endif()

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

# ==========================================
# 终极方案：直接从本地读取压缩包，彻底绕过网络
# ==========================================
include(FetchContent)
FetchContent_Declare(
    raylib
    URL ${CMAKE_CURRENT_SOURCE_DIR}/raylib-5.0.tar.gz
)
FetchContent_MakeAvailable(raylib)
# ==========================================

# Create executable
add_executable(cgame
    src/main.cpp
    src/core/boid.cpp
    src/core/flock.cpp
)

# Link Raylib
target_link_libraries(cgame PRIVATE raylib)

# Include directories
target_include_directories(cgame PRIVATE ${CMAKE_SOURCE_DIR}/src)