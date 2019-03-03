cmake_minimum_required (VERSION 3.5)

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


include(${MCU_LIB_PATH}/lib.cmake)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(version)
include(utils)
include(license)
include(mcu)
include(archive)

# Maps to a solution file (Tutorial.sln). The solution will 
# have all targets (exe, lib, dll) as projects (.vcproj)
project(remcu CXX)

# detect operating system and host processor
message(STATUS "We are on a ${CMAKE_SYSTEM_NAME} system")
message(STATUS "The host processor is ${CMAKE_HOST_SYSTEM_PROCESSOR}")


set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

if(WIN32)
	set(CMAKE_CXX_COMPILER clang-cl)
else()
	set(CMAKE_CXX_COMPILER clang++)
	set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/export)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SOURCE)

if(WIN32)
	set(NET_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/win/winsock2net)
else()
	aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/unix NET_SOURCE)
	SET(CMAKE_CXX_FLAGS "-fno-rtti -fno-exceptions -fvisibility=hidden" )
endif()

set(LIB_SOURCE ${SOURCE} ${NET_SOURCE})


add_subdirectory(test)

# shared library
add_library(remcu SHARED ${LIB_SOURCE})
define_file_basename_for_sources(remcu)
add_dependencies(remcu ${LICENSE_SIGN_FILE})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/export/remcu.h
	DESTINATION  ${CMAKE_CURRENT_BINARY_DIR}/remcu_include
)
file(COPY ${COPY_OBJECT_FROM_LIB}
	DESTINATION  ${CMAKE_CURRENT_BINARY_DIR}/remcu_include
)

add_custom_command(TARGET remcu POST_BUILD
    COMMAND ${ARCHIVE_COMMAND}
    $<TARGET_FILE_NAME:remcu>
    REMCU_LICENSE.txt
    remcu_include
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "---------------------------Archive lib... ---------------------------"
)

if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	target_link_libraries(remcu ws2_32)
endif()