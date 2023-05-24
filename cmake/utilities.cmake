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

function(set_runtime_install_dir_structure target_name)
	get_property(isMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)

	if(isMultiConfig)
		foreach(CONFIG ${CMAKE_CONFIGURATION_TYPES})
			install(TARGETS "${target_name}"
				CONFIGURATIONS "${CONFIG}"
				RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/${CONFIG}"
			)
		endforeach()
	else()
		install(TARGETS "${target_name}"
			RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/${CMAKE_BUILD_TYPE}"
		)
	endif()
endfunction()

function(extract_rpath libraries rpath)
	set(local_rpath "")
	foreach(library "${libraries}")
		get_filename_component(path "${library}" DIRECTORY)
		list(APPEND local_rpath "${path}")
	endforeach()

	set("${rpath}" "${local_rpath}" PARENT_SCOPE)
endfunction()

function(generate_sanitized_library library rpath link_library)
	get_filename_component(library_name "${library}" NAME)
	set(local_library "${CMAKE_BINARY_DIR}/lib/${library_name}")

	add_custom_command(OUTPUT "${local_library}"
		COMMAND "cp" ARGS "${library}" "${local_library}"
		COMMAND "mkdir" ARGS "-p" "${CMAKE_BINARY_DIR}/lib"
		COMMAND "patchelf" ARGS "--set-rpath" "${rpath}" "--force-rpath" "${local_library}"
	)

	get_filename_component(library_target "${library}" NAME_WE)
	add_custom_target("${library_target}" DEPENDS "${local_library}")

	set("${link_library}" "${local_library}" PARENT_SCOPE)
endfunction()

function(generate_sanitized_libraries libraries rpath link_libraries)
	set(local_libraries "")

	foreach(library "${libraries}")
		generate_sanitized_library("${library}" "${rpath}" local_library)
		list(APPEND local_libraries "${local_library}")
	endforeach()

	set("${link_libraries}" "${local_libraries}" PARENT_SCOPE)
endfunction()
