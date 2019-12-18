cmake_minimum_required (VERSION 3.5)
#[[
if(NOT DEFINED CMAKE_CXX_COMPILER)
	message(STATUS "Setting compilator ......")
	if(WIN32)
		set(CMAKE_CXX_COMPILER clang-cl)
	else()
		set(CMAKE_CXX_COMPILER clang++)
		set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s")
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s") #!! -s
	endif()
endif()

# Write information to the console.
message("Running CMakeLists.txt for REMCU...")

if (NOT EXISTS ${CMAKE_BINARY_DIR}/CMakeCache.txt)
  if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
    message("Current type ${CMAKE_BUILD_TYPE} ... set build type to Release")
  endif()
endif()

if(NOT DEFINED ENV{LLVM_ADIN_PATH})
    message(FATAL_ERROR "$LLVM_ADIN_PATH is not defined")
else ()
	set(ADIN_OPT $ENV{LLVM_ADIN_PATH}opt)
endif()

set(CMAKE_VERBOSE_MAKEFILE ON)

set(REMCU_PATH_MK ${CMAKE_CURRENT_SOURCE_DIR}/target.mk)

include(${MCU_LIB_CMAKE})
]]

include(CommonToolchain)

include(GitDesc)
include(utils)
include(license)
#include(archive)

# Maps to a solution file (Tutorial.sln). The solution will 
# have all targets (exe, lib, dll) as projects (.vcproj)
#project(remcu CXX)

# detect operating system and host processor
message(STATUS "We are on a ${CMAKE_SYSTEM_NAME} system")
message(STATUS "The host processor is ${CMAKE_HOST_SYSTEM_PROCESSOR}")


#set(CMAKE_CXX_STANDARD 14)

#set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG -s")
#set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG -s")
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s") #!! -s


aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SOURCE)

if(WIN32)
	set(NET_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/win/winsock2net)
else()
	aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/unix NET_SOURCE)
	# debug
	#set(CMAKE_CXX_FLAGS "-fno-rtti -fno-exceptions -fvisibility=hidden -fPIC" )
endif()

set(LIB_SOURCE ${SOURCE} ${NET_SOURCE})

add_library(remcu_object OBJECT ${LIB_SOURCE} ${CONF_SOURCE})

define_file_basename_for_sources(remcu_object)

target_compile_definitions(remcu_object PRIVATE _LICENSE_TYPE_=\"${LICENSE_TYPE}\")

target_include_directories(remcu_object PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(remcu_object PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/export)
target_include_directories(remcu_object PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if(WIN32)
  target_compile_definitions(remcu_object PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

#add_dependencies(remcu_object IR_TEST_OBJECT_FILE)
#[[
if(MCU_TYPE STREQUAL "TEST")
	add_subdirectory(test EXCLUDE_FROM_ALL)
endif()

if(WIN32)
	target_compile_definitions(-D_CRT_SECURE_NO_WARNINGS)
	target_link_libraries(remcu ws2_32)
endif()

set(ALL_INCLUDE_DIR remcu_include)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/export/remcu.h
	DESTINATION  ${CMAKE_CURRENT_BINARY_DIR}/${ALL_INCLUDE_DIR}
)

copy_target_files(${CMAKE_CURRENT_BINARY_DIR}/${ALL_INCLUDE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_BINARY_DIR}/utils)

set(ARCHIVE_FILES $<TARGET_FILE_NAME:remcu> REMCU_LICENSE.txt ${ALL_INCLUDE_DIR} README.txt)

add_custom_command(TARGET remcu POST_BUILD
    COMMAND ${ARCHIVE_COMMAND}
    ${ARCHIVE_FILES}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "--------------------------- Archive lib ---------------------------"
)
]]

set(LICENSE_SIGN_SCRIPT license.py)
set(LICENSE_SIGN_FILE REMCU_LICENSE.txt)

add_custom_target (
  ${LICENSE_SIGN_FILE}
  COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/${LICENSE_SIGN_SCRIPT}
  -int ${LICENSE_FILE_PATH}
  -out ${ARCHIVE_ROOT_PAT}/${LICENSE_SIGN_FILE}
  -t ${LICENSE_TYPE}
  )


add_dependencies(remcu_object ${LICENSE_SIGN_FILE})


file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/export/remcu.h
	DESTINATION ${ALL_INCLUDE_DIR}
)

execute_process(
	COMMAND ${CMAKE_COMMAND} -E touch 
		${ALL_INCLUDE_DIR}/remcu_exports_symbol_enter.h
		${ALL_INCLUDE_DIR}/remcu_exports_symbol_exit.h
		${ALL_INCLUDE_DIR}/__init__.py
	)



if(NOT DEFINED TARGET_REMCU_OS_NAME)
	if(NOT DEFINED ENV{TARGET_REMCU_OS_NAME})
		message(WARNING "******* $TARGET_REMCU_OS_NAME is not defined! ********")
		set(TARGET_REMCU_OS_NAME "undefined_os")
	else()
		set(TARGET_REMCU_OS_NAME $ENV{TARGET_REMCU_OS_NAME})
	endif()
endif()

if(DEFINED OPTION_DESCRIPTOR)
	message(WARNING "******* set option descriptor ${OPTION_DESCRIPTOR} ********")
else()
	set(OPTION_DESCRIPTOR C_INTERFACE)
endif()

get_git_describe(${CMAKE_CURRENT_SOURCE_DIR} GIT_DESCRIPTION)

get_git_describe(${CMAKE_SOURCE_DIR} GIT_MCU)

set(FULL_VERSION_NAME ${FULL_NAME_MCU_LIB}-${GIT_MCU}-${OPTION_DESCRIPTOR}-${TARGET_REMCU_OS_NAME}-${GIT_DESCRIPTION}-${LICENSE_TYPE})


target_compile_definitions(remcu_object PRIVATE _FULL_VERSION_NAME_=\"${FULL_VERSION_NAME}\")

if(CONSTEXPR_STRING_OBUSFICATION_DISABLE)
	target_compile_definitions(remcu_object PRIVATE CONSTEXPR_STRING_OBUSFICATION_DISABLE)
endif(CONSTEXPR_STRING_OBUSFICATION_DISABLE)


option(LICENSE_FILE_SKIP_ERROR "Skipping error of license file" ON)
if(LICENSE_FILE_SKIP_ERROR)
	target_compile_definitions(remcu_object PRIVATE LICENSE_FILE_SKIP_ERROR)
endif(LICENSE_FILE_SKIP_ERROR)

set(FULL_ARCHIVE_NAME remcu-${FULL_VERSION_NAME} PARENT_SCOPE)
