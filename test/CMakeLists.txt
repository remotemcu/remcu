cmake_minimum_required (VERSION 3.5)

project(Test CXX)

set(MCU_TYPE TEST)

set(MCU_LIB_NAME TEST_LIB)

set(TARGET_REMCU_OS_NAME testing)

set(MCU_MAJOR_VERSION_LIB 1)

set(MCU_MINOR_VERSION_LIB 00)

set(FULL_NAME_MCU_LIB ${MCU_TYPE}-${MCU_LIB_NAME}-${MCU_MAJOR_VERSION_LIB}-${MCU_MINOR_VERSION_LIB} )

set(CONF_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/conf.cpp)

set(CMAKE_MODULE_PATH ${REMCU_VM_PATH}/cmake)

set(LICENSE_TYPE COM)

include(McuCommon)
#[[]]
add_custom_target( MCU_LIB
                   COMMAND make all
                   MCU_UTIL_PATH=${MCU_UTIL_PATH}
                   TARGET_MK=${MCU_INCLUDE_MK_FILE}
                   OUTPUT=${MCU_OBJECT_FILE}
                   CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                   BUILD_DIRECTORY=${BUILD_DIRECTORY}
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                   COMMENT "---------------- Generating ADIN IR ----------------"
)

include(shared)

file(INSTALL ${CMAKE_CURRENT_SOURCE_DIR}/Ir.h
      DESTINATION ${ALL_INCLUDE_DIR}
      )

  file(WRITE
      ${ALL_INCLUDE_DIR}/device_defines.h
      "//test only")

add_executable(test_shared test.cpp)
target_include_directories(test_shared PRIVATE ${ALL_INCLUDE_DIR})
target_link_libraries(test_shared remcu)

if(NOT DEFINED WIN32 AND ${CMAKE_VERSION} VERSION_GREATER "3.8.0")
      enable_testing()
      add_subdirectory(gtest)
endif()