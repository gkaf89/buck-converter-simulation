# Set the executable along with the required source files
add_executable(hybrid_evolution_tutorial hybrid_evolution_tutorial.cpp)

# Instruct to link against the ariadne library
target_link_libraries(hybrid_evolution_tutorial
	PRIVATE "${ARIADNE_LIBRARIES}"
	PRIVATE "${MPFR_LIBRARIES}" # Link the local library so that the correct dependency appears in RPATH
	PRIVATE "${GMP_LIBRARIES}" # Link the local library so that the correct dependency appears in RPATH
)

# Use RPATH over RUNPATH so that the correct libraries are preferred over system ones
target_link_options(hybrid_evolution_tutorial PUBLIC "-Wl,--disable-new-dtags")

target_include_directories(hybrid_evolution_tutorial
	PRIVATE "${ARIADNE_INCLUDES}"
	PRIVATE "${MPFR_INCLUDES}"
	PRIVATE "${GMP_INCLUDES}"
)

# Define installation diretory
install(TARGETS hybrid_evolution_tutorial
	RUNTIME DESTINATION "${INSTALL_BINDIR}"
)
