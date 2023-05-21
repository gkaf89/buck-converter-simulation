# Set the executable along with the required source files
add_executable(hybrid_evolution_tutorial hybrid_evolution_tutorial.cpp)

# Instruct to link against the ariadne library
target_link_libraries(hybrid_evolution_tutorial ${ARIADNE_LIBRARIES})

# Define installation diretory
set_runtime_install_dir_structure(hybrid_evolution_tutorial)
