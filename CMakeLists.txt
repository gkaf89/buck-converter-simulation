# Preferable to avoid warnings during execution
cmake_minimum_required(VERSION 3.9)

# Add directory with utility functions and modules in the serach path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Include basic utility fuinctions
include(utilities)

project(BuckConverter VERSION 0.1)

# Set the C++ standard to C++20, as used by Ariadne
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set_build_type_parameters()
set_output_dir_structure()

# Find Ariadne
find_package(Ariadne REQUIRED)

# Add the headers to the includes
include_directories(SYSTEM ${ARIADNE_INCLUDES})

# Recipies for building sources
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src")
