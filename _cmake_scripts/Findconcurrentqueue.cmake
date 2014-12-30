include( utils )

if (concurrentqueue_ROOT)
	set(concurrentqueue_root ${concurrentqueue_ROOT} )
elseif(NOT "$ENV{CONCURRENTQUEUE_ROOT}" STREQUAL "")
	set(concurrentqueue_root $ENV{CONCURRENTQUEUE_ROOT} )
else()
	message(FATAL_ERROR "[ERROR]: No CONCURRENTQUEUE_ROOT environment variable found")
endif()

if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
	set(CMAKE_BUILD_TYPE Debug)
	if (!concurrentqueue_FIND_QUIETLY)
		message(STATUS "CMAKE_BUILD_TYPE was not tuned, so we install there 'Debug'")
	endif()
else()
endif()

if (concurrentqueue_DEBUG)
	message(STATUS "We going to look into '${concurrentqueue_root}' (CONCURRENTQUEUE_ROOT environment variable) folder.")
	create_string_from_list( component_list ${concurrentqueue_FIND_COMPONENTS} )
	message(STATUS "We are looking for: '" ${component_list} "' components.")
endif(concurrentqueue_DEBUG)

set(concurrentqueue_FOUND FALSE)
#set(concurrentqueue_INCLUDE_DIRS "")
#set(concurrentqueue_LIBRARIES "")
set(concurrentqueue_LIBRARIES NOTFOUND)

find_path( concurrentqueue_INCLUDE_DIRS
		NAMES "concurrentqueue.h"
		HINTS "${concurrentqueue_root}/")

if(NOT concurrentqueue_INCLUDE_DIRS )
	message(FATAL_ERROR " We can't find 'concurrentqueue.h' component")
endif()


if (concurrentqueue_DEBUG)
	create_string_from_list(headers ${concurrentqueue_INCLUDE_DIRS})
	message(STATUS "concurrentqueue_INCLUDE_DIRS: " ${headers})
	create_string_from_list(libraries ${concurrentqueue_LIBRARIES})
	message(STATUS "concurrentqueue_LIBRARIES: " ${libraries})
endif(concurrentqueue_DEBUG)

