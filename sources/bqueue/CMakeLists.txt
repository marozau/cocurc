find_package( Boost 1.54 )

compile_project( ${module_name} "*.cpp" "*.h" STATIC libraries Boost )

