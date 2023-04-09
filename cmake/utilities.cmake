macro(set_output_dir_structure_single_config)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}" PARENT_SCOPE)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}" PARENT_SCOPE)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}" PARENT_SCOPE)
endmacro()

macro(set_output_dir_structure_multiple_config)
	foreach(CONFIG ${CMAKE_CONFIGURATION_TYPES})
		string(TOUPPER "${CONFIG}" CONFIG_SUFFIX)

		set("CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIG_SUFFIX}" "${CMAKE_BINARY_DIR}/${CONFIG}/${CMAKE_INSTALL_BINDIR}" PARENT_SCOPE)
		set("CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CONFIG_SUFFIX}" "${CMAKE_BINARY_DIR}/${CONFIG}/${CMAKE_INSTALL_LIBDIR}" PARENT_SCOPE)
		set("CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CONFIG_SUFFIX}" "${CMAKE_BINARY_DIR}/${CONFIG}/${CMAKE_INSTALL_LIBDIR}" PARENT_SCOPE)
	endforeach()
endmacro()

function(set_output_dir_structure)
	include(GNUInstallDirs)

	get_property(isMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)

	if(isMultiConfig)
		set_output_dir_structure_multiple_config()
	else()
		set_output_dir_structure_single_config()
	endif()
endfunction()
