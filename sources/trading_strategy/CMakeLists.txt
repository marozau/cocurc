find_package( Boost 1.54 ${SEARCH_PARAMETERS} COMPONENTS
	filesystem
	thread
	regex
	date_time
	system
)

compile_project( ${module_name} "*.cpp" "*.h" STATIC libraries Boost bqueue runner state_dispatcher timed_idle queue_dispatcher session )

