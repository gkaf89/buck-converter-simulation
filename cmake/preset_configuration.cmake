if(PRESET_NAME STREQUAL "Release")
	message(STATUS "Building a 'Release' build...")	
	set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Set the configuration type to 'Release'" FORCE)
elseif(PRESET_NAME STREQUAL "Debug")
	message(STATUS "Building a 'Debug' build...")
	set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Set the configuration type to 'Debug'" FORCE)
else()
	message(FATAL_ERROR "Unsupported preset")
endif()

set(CMAKE_INSTALL_PREFIX "$ENV{HOME}/usr/opt/buck-converter-simulator/${PRESET_NAME}" CACHE PATH "Local installation path")

set(CMAKE_PREFIX_PATH "$ENV{HOME}/opt/ariadne/${PRESET_NAME}/cxx;$ENV{HOME}/lib/mpfr;$ENV{HOME}/lib/gmp" CACHE PATH "Search path for locally installed Ariadne, MPFR, and GMP librarires" FORCE)
