add_executable(python-proto-tests catch_main.cpp)
_pp__link_static(python-proto-tests)

target_link_libraries(python-proto-tests
	PRIVATE
		Catch2::Catch2
		platypus-proto
)

target_sources(python-proto-tests
	PRIVATE
		util.cpp
)

include(CTest)
catch_discover_tests(python-proto-tests)
