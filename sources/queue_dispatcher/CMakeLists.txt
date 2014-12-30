find_package( Boost 1.54 )

find_package( concurrentqueue )

compile_project( ${module_name} "*.cpp" "*.h" STATIC libraries Boost bqueue runner concurrentqueue )

