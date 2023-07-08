macro(set_output_dir_structure_single_config)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}" PARENT_SCOPE)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}" PARENT_SCOPE)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}" PARENT_SCOPE)
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

function(set_runtime_install_dir_structure target_name)
	install(TARGETS "${target_name}"
		RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/${CMAKE_BUILD_TYPE}"
	)
endfunction()

function(extract_library_names library_paths libraries)
	set(local_libraries "")
	foreach(library_path ${library_paths})
		get_filename_component(library "${library_path}" NAME)
		list(APPEND local_libraries "${library}")
	endforeach()

	set("${libraries}" "${local_libraries}" PARENT_SCOPE)
endfunction()

function(extract_rpath library_paths rpath)
	set(local_rpath "")
	foreach(library ${library_paths})
		get_filename_component(path "${library}" DIRECTORY)
		list(APPEND local_rpath "${path}")
        endforeach()
 
	set("${rpath}" "${local_rpath}" PARENT_SCOPE)
endfunction()

function(set_rpath local_target_file needed_library_paths patch_command)
	extract_rpath("${needed_library_paths}" rpath)

	string(REPLACE ";" ":" posix_rpath "${rpath}")

	set(patch_command "COMMAND" ":" "&&"
		"patchelf" "--set-rpath" "${posix_rpath}" "--force-rpath" "${local_target_file}"
		PARENT_SCOPE
	)
endfunction()

function(set_needed_libraries target_file local_target_file needed_library_paths patch_command)
	extract_library_names("${needed_library_paths}" needed_libraries)

	set(local_patch_command "")
	foreach(library ${needed_libraries})
		list(APPEND local_patch_command "COMMAND"  ":" "&&"
			"{" 
				"(" "readelf" "-d" "${target_file}" 
					"|" "grep" "\"(NEEDED)\"" 
					"|" "grep" "${library}" "1>/dev/null"
				")" 
				"||" 
				"patchelf" "--add-needed" "${library}" "${local_target_file}"
			"\;" "}"
		)
	endforeach()

	set("${patch_command}" "${local_patch_command}" PARENT_SCOPE)
endfunction()

function(generate_test_binary target needed_library_paths)
	set(local_target "LOCAL_${target}")
	set(local_target_file "${CMAKE_BINARY_DIR}/local/${CMAKE_INSTALL_BINDIR}/${target}")
	set(target_file "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${target}")

	set_rpath("${local_target_file}" "${needed_library_paths}" patch_rpath_command)

	set_needed_libraries("${target_file}" "${local_target_file}" "${needed_library_paths}" patch_needed_libraries_command)

	add_custom_command(OUTPUT "${local_target_file}"
		DEPENDS "$<TARGET_FILE:${target}>"
		COMMAND "cp" "$<TARGET_FILE:${target}>" "${local_target_file}"
		${patch_rpath_command}
		${patch_needed_libraries_command}
	)

	add_custom_target("${local_target}" ALL DEPENDS "${local_target_file}")
endfunction()
