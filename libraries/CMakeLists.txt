# catch
if(${is_root_project})
	add_subdirectory(catch)
	include(${CMAKE_CURRENT_SOURCE_DIR}/../libraries/catch/contrib/Catch.cmake)
endif()

# protobuf
set(protobuf_BUILD_TESTS OFF)
set(protobuf_BUILD_EXAMPLES OFF)
set(BUILD_SHARED_LIBS OFF)
set(protobuf_BUILD_SHARED_LIBS OFF)
set(protobuf_WITH_ZLIB OFF)

add_subdirectory(protobuf)

_pp__ignore_warnings(libprotobuf)
_pp__link_static(libprotobuf)
set_target_properties(libprotobuf
	PROPERTIES
	FOLDER libraries
)

_pp__ignore_warnings(libprotobuf-lite)
_pp__link_static(libprotobuf-lite)
set_target_properties(libprotobuf-lite
	PROPERTIES
	EXCLUDE_FROM_ALL true
	EXCLUDE_FROM_DEFAULT_BUILD true
	FOLDER libraries
)

_pp__ignore_warnings(libprotoc)
_pp__link_static(libprotoc)
set_target_properties(libprotoc
	PROPERTIES
	EXCLUDE_FROM_ALL true
	EXCLUDE_FROM_DEFAULT_BUILD true
	FOLDER libraries
)

_pp__ignore_warnings(protoc)
_pp__link_static(protoc)
set_target_properties(protoc
	PROPERTIES
	FOLDER libraries
)
