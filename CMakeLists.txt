cmake_minimum_required(VERSION 3.10)
project(WilltBotAMRSimulation VERSION 0.1 LANGUAGES CXX)

# C++ settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)
add_compile_options(-O3 -Wall -Wextra -Wpedantic)

# Find SDL2
find_package(SDL2 REQUIRED)

# Collect sources first
file(GLOB SOURCES "src/*.cpp")

# Define the executable target FIRST
add_executable(willtbot ${SOURCES})

# NOW configure includes and links for the target
target_include_directories(willtbot BEFORE PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(willtbot PRIVATE
    ${SDL2_LIBRARIES}
)