# Preferable to avoid warnings during execution
cmake_minimum_required(VERSION 3.9)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if (DEFINED PRESET_NAME)
	include(preset_configuration)
endif()

project(BuckConverter VERSION 0.1)

# Set to search the root source directory for the FindAriadne.cmake module
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}")

# Set the C++ standard to C++20, as used by Ariadne
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if( CMAKE_BUILD_TYPE STREQUAL "Release" )
	message(STATUS "Configuring a release build...")
	message(STATUS "CXX flags: ${CMAKE_CXX_FLAGS_RELEASE}")
elseif( CMAKE_BUILD_TYPE STREQUAL "Debug" )
	message(STATUS "Configuring a debug build...")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
	message(STATUS "CXX flags: ${CMAKE_CXX_FLAGS_DEBUG}")
endif()

# Set output directory structure
include(utilities)
set_output_dir_structure()

# Find Ariadne
find_package(Ariadne REQUIRED)

# Add the headers to the includes
include_directories(SYSTEM ${ARIADNE_INCLUDES})

# Recipies for building sources
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src")
