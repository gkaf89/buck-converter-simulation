# Set the executable along with the required source files
add_executable(hybrid_evolution_tutorial hybrid_evolution_tutorial.cpp)

# Instruct to link against the ariadne library
extract_rpath("${MPFR_LIBRARIES}" MPFR_RPATH)
target_link_sanitized_libraries(hybrid_evolution_tutorial "${ARIADNE_LIBRARIES}" "${MPFR_RPATH}")

# Define installation diretory
set_runtime_install_dir_structure(hybrid_evolution_tutorial)
