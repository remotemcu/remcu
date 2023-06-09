if (NOT EXISTS ${CMAKE_BINARY_DIR}/CMakeCache.txt)
  if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
    message("Current type ${CMAKE_BUILD_TYPE} ... set build type to Release")
  endif()
endif()

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
	message(error "not defined toolchain file")
endif()


message("CMAKE_C_FLAGS_RELEASE : ${CMAKE_C_FLAGS_RELEASE} ")
message("CMAKE_CXX_FLAGS_RELEASE : ${CMAKE_CXX_FLAGS_RELEASE} ")
