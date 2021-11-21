cmake_minimum_required(VERSION 3.18)

project(
	Platypus-Proto
	VERSION 0.1
	DESCRIPTION "Support lib for Platypus engine. Handles protobuf related features."
	LANGUAGES CXX
)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
	include(cmake/config.cmake)

	# # Docs only available if this is the main app
	# find_package(Doxygen)
	# if(Doxygen_FOUND)
	# 	add_subdirectory(docs)
	# else()
	# 	message(STATUS "Doxygen not found, not building docs")
	# endif()

endif()

enable_testing()
include(cmake/funcs.cmake)
add_subdirectory(libraries)

add_library(platypus-proto STATIC
	source/util.hpp
)
_pp__link_static(platypus-proto)

source_group(source FILES source/util.hpp)

target_include_directories(platypus-proto
	INTERFACE
		${CMAKE_CURRENT_SOURCE_DIR}/source
		${CMAKE_CURRENT_BINARY_DIR}/generated/proto/cpp
		${CMAKE_CURRENT_BINARY_DIR}/generated/proto/cpp/include
)

target_link_libraries(platypus-proto
	libprotobuf
)

set(proto_files
	source/schema/settings.proto
)
source_group(source\\schema FILES ${proto_files})

set(proto_root_dir "${CMAKE_CURRENT_SOURCE_DIR}/source/schema")
set(proto_output_dir "${CMAKE_CURRENT_BINARY_DIR}/generated/proto")

foreach(proto_file ${proto_files})
	_pp__compile_proto(${proto_file} ${proto_root_dir} ${proto_output_dir} custom_target
		generated_cpp
		generated_cpp_wrapper
		generated_csharp
		generated_binary
	)

	set_target_properties(${custom_target}
		PROPERTIES
		FOLDER proto_gen
	)

	add_dependencies(platypus-proto ${custom_target})
	target_sources(platypus-proto
		PRIVATE
			${proto_file}
			${generated_cpp}
			${generated_cpp_wrapper}
	)
	source_group(generated FILES ${generated_cpp})
	source_group(generated\\include FILES ${generated_cpp_wrapper})
endforeach()

add_subdirectory(tests)
