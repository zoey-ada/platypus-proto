add_executable(platypus-proto-tests catch_main.cpp)
_pp__link_static(platypus-proto-tests)

target_link_libraries(platypus-proto-tests
	PRIVATE
		Catch2::Catch2
		platypus-proto
)

target_sources(platypus-proto-tests
	PRIVATE
		util.cpp
)

include(CTest)
catch_discover_tests(platypus-proto-tests)
