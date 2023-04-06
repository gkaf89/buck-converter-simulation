# Find MPFR

find_library(MPFR_LIBRARIES mpfr)
find_path(MPFR_INCLUDE_DIRS mpfr.h)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MPFR DEFAULT_MSG MPFR_LIBRARIES MPFR_INCLUDE_DIRS)

mark_as_advanced(
	MPFR_INCLUDE_DIRS
	MPFR_LIBRARIES
)
