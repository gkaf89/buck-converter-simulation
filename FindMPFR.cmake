# Find MPFR

find_library(MPFR_LIBRARY mpfr)
find_path(MPFR_INCLUDE_DIR mpfr.h)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MPFR DEFAULT_MSG MPFR_LIBRARY MPFR_INCLUDE_DIR)

mark_as_advanced(
	ARIADNE_INCLUDE_DIRS
	ARIADNE_LIBRARIES
)
