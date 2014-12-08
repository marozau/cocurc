find_package( Boost 1.54 ${SEARCH_PARAMETERS} COMPONENTS
	filesystem
	thread
	regex
	date_time
	chrono
	system
	unit_test_framework
)

compile_project( ${tests_name} "*.cpp" "*.h" BINARY tests ${module_name} boost_dynamic_test_helper Boost )
register_test( ${tests_name} 20.0 20.0 )

