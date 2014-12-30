find_package( Boost 1.54 ${SEARCH_PARAMETERS} COMPONENTS
	filesystem
	thread
	regex
	date_time
	system
)

find_package( concurrentqueue )

compile_project( ${module_name} "*.cpp" "*.h" STATIC libraries Boost bqueue runner concurrentqueue )

