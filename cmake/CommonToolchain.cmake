if (NOT EXISTS ${CMAKE_BINARY_DIR}/CMakeCache.txt)
  if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
    message("Current type ${CMAKE_BUILD_TYPE} ... set build type to Release")
  endif()
endif()


if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
	message(STATUS "Setting TOOLCHAIN --->")
	message(STATUS "Redefine compilator ${CMAKE_CXX_COMPILER} on ... ")
	if(WIN32)
		set(CMAKE_CXX_COMPILER clang-cl)
	else()
		set(CMAKE_CXX_COMPILER clang++)
	endif()
	message(STATUS ".... ${CMAKE_CXX_COMPILER} ")

	set(CMAKE_CXX_STANDARD 14)

	set(MCU_INCLUDE_MK_FILE ${REMCU_VM_PATH}/mcu_utils/pc.mk)
	if(WIN32)
		set(STRIP_COMMAND echo)
	else()
		set(STRIP_COMMAND strip)
	endif()
else()
	message(STATUS "CMAKE_TOOLCHAIN_FILE : ${CMAKE_TOOLCHAIN_FILE}")
endif()


set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG -pthread")
set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG")

if(WIN32)
else()
	set(CMAKE_CXX_FLAGS "-fno-rtti -fno-exceptions -fPIC" )
	set(CMAKE_C_FLAGS "-fPIC" )
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fvisibility=hidden -s")
	set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fvisibility=hidden -s")
endif()

message("CMAKE_C_FLAGS_RELEASE : ${CMAKE_C_FLAGS_RELEASE} ")
message("CMAKE_CXX_FLAGS_RELEASE : ${CMAKE_CXX_FLAGS_RELEASE} ")
