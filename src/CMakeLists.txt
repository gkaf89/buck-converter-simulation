# Set the executable along with the required source files
add_executable(hybrid_evolution_tutorial hybrid_evolution_tutorial.cpp)

# Instruct to link against the ariadne library
set(hybrid_evolution_tutorial_libraries "")
list(APPEND hybrid_evolution_tutorial_libraries "${ARIADNE_LIBRARIES}")
list(APPEND hybrid_evolution_tutorial_libraries "${MPFR_LIBRARIES}")
list(APPEND hybrid_evolution_tutorial_libraries "${GMP_LIBRARIES}")
target_link_libraries(hybrid_evolution_tutorial "${hybrid_evolution_tutorial_libraries}")

target_include_directories(hybrid_evolution_tutorial
	PRIVATE "${ARIADNE_INCLUDES}"
	PRIVATE "${MPFR_INCLUDES}"
	PRIVATE "${GMP_INCLUDES}"
)

generate_test_binary(hybrid_evolution_tutorial "${hybrid_evolution_tutorial_libraries}")

# Define installation diretory
install(TARGETS hybrid_evolution_tutorial
	RUNTIME DESTINATION "${INSTALL_BINDIR}"
)
