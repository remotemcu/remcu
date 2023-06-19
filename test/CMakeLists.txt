
if(DEFINED REMCU_PREBUILD_CONFIG_FILE)
      include(REMCU_PREBUILD_CONFIG_FILE)
endif()

project(Test CXX)

set(MCU_TYPE TEST)

set(MCU_LIB_NAME TEST_LIB)

set(TARGET_REMCU_OS_NAME testing)

set(MCU_MAJOR_VERSION_LIB 1)

set(MCU_MINOR_VERSION_LIB 00)

include(${REMCU_VM_PATH}/cmake/mcu_build_target.cmake)

file(INSTALL ${CMAKE_CURRENT_SOURCE_DIR}/Ir.h
      DESTINATION ${ALL_INCLUDE_DIR}
      )

  file(WRITE
      ${ALL_INCLUDE_DIR}/device_defines.h
      "//test only")

add_executable(test_shared test.cpp)
target_include_directories(test_shared PRIVATE ${ALL_INCLUDE_DIR})
target_link_libraries(test_shared remcu)

# if(NOT DEFINED WIN32 AND ${CMAKE_VERSION} VERSION_GREATER "3.8.0")
#       enable_testing()
#       add_subdirectory(gtest)
# endif()