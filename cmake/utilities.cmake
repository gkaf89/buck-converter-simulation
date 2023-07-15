macro(set_output_dir_structure_single_config)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}" PARENT_SCOPE)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}" PARENT_SCOPE)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}" PARENT_SCOPE)

	set(INSTALL_BINDIR "${CMAKE_INSTALL_PREFIX}/${CMAKE_BUILD_TYPE}/bin" PARENT_SCOPE)
endmacro()

function(set_output_dir_structure)
	include(GNUInstallDirs)

	get_property(isMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)

	if(isMultiConfig)
		message(FATAL_ERROR "This project does not support multi-configuration generators.")
	else()
		set_output_dir_structure_single_config()
	endif()
endfunction()
