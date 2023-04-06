# Find MPFR
#
# Try to find the MPFR library
#
# On completion the following variables will be defined
# 
# MPFR_FOUND - system has MPFR
# MPFR_INCLUDE_DIRS - the MPFR include directories
# MPFR_LIBRARIES - Link these to use MPFR

find_library(MPFR_LIBRARIES mpfr)
find_path(MPFR_INCLUDE_DIRS mpfr.h)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MPFR DEFAULT_MSG MPFR_LIBRARIES MPFR_INCLUDE_DIRS)

mark_as_advanced(
	MPFR_INCLUDE_DIRS
	MPFR_LIBRARIES
)
