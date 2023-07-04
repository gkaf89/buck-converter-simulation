# Set the executable along with the required source files
add_executable(hybrid_evolution_tutorial hybrid_evolution_tutorial.cpp)

# Instruct to link against the ariadne library
extract_rpath("${GMP_LIBRARIES}" GMP_RPATH)
extract_rpath("${MPFR_LIBRARIES}" MPFR_RPATH)

set(ariadne_link_libraries_source "")
list(APPEND ariadne_link_libraries_source "${GMP_RPATH}")
list(APPEND ariadne_link_libraries_source "${MPFR_RPATH}")

generate_sanitized_libraries("${ARIADNE_LIBRARIES}" "${ariadne_link_libraries_source}" ariadne_link_libraries_sanitized)

target_link_libraries(hybrid_evolution_tutorial "${ariadne_link_libraries_sanitized}")

# Define installation diretory
set_runtime_install_dir_structure(hybrid_evolution_tutorial)
