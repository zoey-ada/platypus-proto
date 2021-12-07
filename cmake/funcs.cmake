function(_pp__compile_proto proto_file include_dir out_dir __out_custom_target
	__out_generated_cpp __out_generated_cpp_wrapper __out_generated_csharp __out_generated_binary)

	get_filename_component(proto_name ${proto_file} NAME_WLE)
	get_filename_component(proto_file_abs ${proto_file} ABSOLUTE)

	set(proto_file_full_path "${proto_file_abs}")
	set(generated_bin "${out_dir}/bin/${proto_name}.bin")

	set(generaged_cpp_root "${bin_dir}/include/platypus_proto")
	set(generated_cpp_wrapper "${generaged_cpp_root}/${proto_name}.hpp")
	set(generated_cpp_header "${generaged_cpp_root}/gen/${proto_name}.pb.h")
	set(generated_cpp_impl "${generaged_cpp_root}/gen/${proto_name}.pb.cc")

	set(generated_cs "${out_dir}/cs/${proto_name}.cs")
	set(generated_java "${out_dir}/java/${proto_name}.java")
	set(generated_js "${out_dir}/js/${proto_name}.js")
	set(generated_kotlin "${out_dir}/kotlin/${proto_name}.kt")
	set(generated_php "${out_dir}/php/${proto_name}.php")
	set(generated_ruby "${out_dir}/ruby/${proto_name}.rb")
	set(generated_python "${out_dir}/python/${proto_name}.py")
	set(generated_obj_c_header "${out_dir}/obj_c/${proto_name}.h")
	set(generated_obj_c_method "${out_dir}/obj_c/${proto_name}.m")

	set(generated_files ${generated_cpp_header} ${generated_cpp_impl} ${generated_cs} ${generated_bin})

	add_custom_command(
		OUTPUT ${generated_files}
		COMMENT "generating c++/c#/descriptor from ${proto_file}"

		DEPENDS ${proto_file_full_path}
		DEPENDS protoc

		COMMAND protoc
			--proto_path="${include_dir}"
			--proto_path="${PROTOBUF_INCLUDE_PATH}"
			--cpp_out="${generaged_cpp_root}/gen"
			--csharp_out="${out_dir}/cs"
			--descriptor_set_out="${generated_bin}"
			# --java_out="${out_dir}/java"
			# --js_out="${out_dir}/js"
			# --kotlin_out="${out_dir}/kotlin"
			# --objc_out="${out_dir}/obj_c"
			# --php_out="${out_dir}/php"
			# --python_out="${out_dir}/python"
			# --ruby_out="${out_dir}/ruby"
			"${proto_file_full_path}"
	)

	# generate wrapper to disable warnings
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/proto.hpp.in ${generated_cpp_wrapper})
	set(generated_sources ${generated_sources} ${out_dir}/${proto_name}.hpp)

	add_custom_target("generated-${proto_name}"
		DEPENDS ${generated_files} ${generated_wrapper}
	)

	set(${__out_custom_target} "generated-${proto_name}" PARENT_SCOPE)

	set(${__out_generated_cpp}
		${generated_cpp_impl}
		${generated_cpp_header}
		${generated_cpp_wrapper}

		PARENT_SCOPE
	)

	set(${__out_generated_cpp_wrapper}
		${generated_cpp_wrapper}

		PARENT_SCOPE
	)

	set(${__out_generated_csharp}
		${generated_cs}

		PARENT_SCOPE
	)

	set(${__out_generated_binary}
		${generated_bin}

		PARENT_SCOPE
	)
endfunction()

function(_pp__warnings_as_errors target)
	if(MSVC)
		# Force to always compile with W4
		target_compile_options(${target} PRIVATE /W4 /WX)
	elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
		# Update if necessary
		target_compile_options(${target} PRIVATE -Werror -Wall -Wextra -Wpedantic)
	endif()
endfunction()

function(_pp__ignore_warnings target)
	if(MSVC)
		target_compile_options(${target} PRIVATE /w)
	else()
		target_compile_options(${target} PRIVATE -w)
	endif()
endfunction()

function(_pp__link_static target)
	set_property(TARGET ${target}
		PROPERTY
		MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
	)
endfunction()

function(_pp__link_dynamic target)
	set_property(TARGET ${target}
		PROPERTY
		MSVC_RUNTIME_LIBRARY "MultiThreadedDLL$<$<CONFIG:Debug>:Debug>"
	)
endfunction()
