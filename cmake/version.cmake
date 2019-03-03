find_package (Git)

if (GIT_FOUND)
	message("git found: ${GIT_EXECUTABLE} in version ${GIT_VERSION_STRING}")
	execute_process(
		COMMAND ${GIT_EXECUTABLE} describe --tags --abbrev=0
		OUTPUT_VARIABLE LAST_TAG)

	string (REGEX MATCHALL "([^/]+)[\^{}]\n" TEMP ${LAST_TAG})
	string (REGEX REPLACE "\n" ""  LAST_TAG ${TEMP})

	execute_process(
	    COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
	    OUTPUT_VARIABLE COMMIT_HASH)

	string (REGEX MATCHALL "([^/]+)[\^{}]\n" TEMP ${COMMIT_HASH})
	string (REGEX REPLACE "\n" ""  COMMIT_HASH ${TEMP})

	message("git : ${LAST_TAG} @ ${COMMIT_HASH}")
	add_definitions(-D_GIT_HASH_=\"${COMMIT_HASH}\")
	add_definitions(-D_GIT_TAG_=\"${LAST_TAG}\" )
else()
	add_definitions(-DCOMMIT_HASH=hz)
	add_definitions(-DLAST_TAG=hz)
endif (GIT_FOUND)


string(TIMESTAMP _TIMESTAMP_)
add_definitions(-D_TIMESTAMP_=\"${_TIMESTAMP_}\" )

if(NOT DEFINED CMAKE_REMCU_OS_NAME)
	if(NOT DEFINED ENV{CMAKE_REMCU_OS_NAME})
		message(FATAL_ERROR "******* $CMAKE_REMCU_OS_NAME is not defined! ********")
	else()
		set(CMAKE_REMCU_OS_NAME $ENV{CMAKE_REMCU_OS_NAME})
	endif()
endif()

if(OPTION_DESCRIPTOR)
		message(WARNING "******* set option descriptor ${OPTION_DESCRIPTOR} ********")
else()
	set(OPTION_DESCRIPTOR C)
endif()

set(FULL_VERSION_NAME ${FULL_NAME_MCU_LIB}-${OPTION_DESCRIPTOR}-${CMAKE_REMCU_OS_NAME}-${LAST_TAG}-${COMMIT_HASH}-${LICENSE_TYPE})

add_definitions(-D_FULL_VERSION_NAME_=\"${FULL_VERSION_NAME}\")

set(FULL_ARCHIVE_NAME remcu-${FULL_VERSION_NAME})