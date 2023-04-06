# Preferable to avoid warnings during execution
cmake_minimum_required(VERSION 3.9)

project(HybridEvolutionTutorial VERSION 2.3)

# Set to search the root source directory for the FindAriadne.cmake module
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}")

# Set the C++ standard to C++20, as used by Ariadne
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Set output directory structure
include(utilities)
set_output_dir_structure()

# Find Ariadne
find_package(Ariadne REQUIRED)

# Add the headers to the includes
include_directories(SYSTEM ${ARIADNE_INCLUDE_DIRS})

# Recipies for building sources
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src")
