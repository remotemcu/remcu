cmake_minimum_required (VERSION 3.5)

# Write information to the console.
message("Running CMakeLists.txt for REMCU...")
#[[]]
if (NOT EXISTS ${CMAKE_BINARY_DIR}/CMakeCache.txt)
  if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
    message("Current type ${CMAKE_BUILD_TYPE} ... set build type to Release")
  endif()
endif()

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(version)

# Maps to a solution file (Tutorial.sln). The solution will 
# have all targets (exe, lib, dll) as projects (.vcproj)
project(remcu-lib CXX)

# detect operating system and host processor
message(STATUS "We are on a ${CMAKE_SYSTEM_NAME} system")
message(STATUS "The host processor is ${CMAKE_HOST_SYSTEM_PROCESSOR}")

set(CMAKE_CXX_STANDARD 14)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/export)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SOURCE)

if(WIN32)
	set(NET_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/win/winsock2net)
else()
	aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/unix NET_SOURCE)
	SET(CMAKE_CXX_FLAGS "-fno-rtti -fno-exceptions" )
endif()

set(LIB_SOURCE ${SOURCE} ${NET_SOURCE})

set(ADIN_OPT ${LLVM_ADIN_PATH}opt)

set(WIN_CLANG_IR_OPTIONS -D_MT -D_DLL)

add_subdirectory(test)

# shared library
add_library(remcu_dll SHARED ${LIB_SOURCE})
	


if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	target_link_libraries(remcu_dll ws2_32)
endif()