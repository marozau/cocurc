include( utils )

if (system_utilities_ROOT)
	set(cocurc_root ${cocurc_ROOT} )
elseif(NOT "$ENV{SYSTEM_UTILITIES_ROOT}" STREQUAL "")
	set(cocurc_root $ENV{CONCURC_ROOT} )
else()
	message(FATAL_ERROR "[ERROR]: No CONCURC_ROOT environment variable found")
endif()

if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
	set(CMAKE_BUILD_TYPE Debug)
	if (!cocurc_FIND_QUIETLY)
		message(STATUS "CMAKE_BUILD_TYPE was not tuned, so we install there 'Debug'")
	endif()
else()
endif()

if (cocurc_DEBUG)
	message(STATUS "We going to look into '${cocurc_root}' (CONCURC_ROOT environment variable) folder.")
	create_string_from_list( component_list ${cocurc_FIND_COMPONENTS} )
	message(STATUS "We are looking for: '" ${component_list} "' components.")
endif(cocurc_DEBUG)

set(cocurc_FOUND FALSE)
set(cocurc_INCLUDE_DIRS "")
set(cocurc_LIBRARIES "")

if (UNIX)
	set(binary_search_folder ${cocurc_root}/_build_${CMAKE_BUILD_TYPE}_${CMAKE_ADDRESS_MODEL}/bin_${CMAKE_ADDRESS_MODEL}/${CMAKE_BUILD_TYPE})
else(UNIX)
	set(binary_search_folder ${cocurc_root}/_build_${CMAKE_BUILD_TYPE}_${CMAKE_ADDRESS_MODEL}/bin_${CMAKE_ADDRESS_MODEL}/${CMAKE_BUILD_TYPE})
endif(UNIX)
if (NOT cocurc_FIND_QUIETLY)
	message(STATUS "Looking for libraries at " ${binary_search_folder})
endif()

if (cocurc_DEBUG)
	message(STATUS "Binary search folder: " ${binary_search_folder} )
endif(cocurc_DEBUG)

set(components bqueue runner state_dispatcher )

foreach(component ${cocurc_FIND_COMPONENTS})

list_contains(we_should_find_${component} ${component} ${components})
if(we_should_find_${component})

	if (cocurc_DEBUG)
		message(STATUS "Searching for ${component} library. ")
	endif()
	set(cocurc_${component}_FOUND FALSE)
	set(cocurc_${component}_LIBRARIES NOTFOUND)

	find_library( cocurc_${component}_LIBRARY
		NAMES "${component}" 
		HINTS ${binary_search_folder} )
	set(cocurc_${component}_LIBRARIES ${cocurc_${component}_LIBRARY})

	if(NOT cocurc_FIND_QUIETLY)
		message(STATUS "cocurc ${component} component: ")
		create_string_from_list(libraries ${cocurc_${component}_LIBRARIES})
		message(STATUS "   libraries:  " ${libraries})
	endif()	

	if(NOT cocurc_${component}_LIBRARY)
		message(FATAL_ERROR " We can't find cocurc ${component} component")
	else()
		set(cocurc_LIBRARIES ${cocurc_LIBRARIES} ${cocurc_${component}_LIBRARIES})
	endif()
else()
	message(FATAL_ERROR "Unknown component: ${component}")
endif(we_should_find_${component})

endforeach( component )

set(blogger_INCLUDE_DIRS ${blogger_INCLUDE_DIRS} ${cocurc_root})

if (cocurc_DEBUG)
	create_string_from_list(headers ${cocurc_INCLUDE_DIRS})
	message(STATUS "cocurc_INCLUDE_DIRS: " ${headers})
	create_string_from_list(libraries ${cocurc_LIBRARIES})
	message(STATUS "cocurc_LIBRARIES: " ${libraries})
endif(cocurc_DEBUG)

