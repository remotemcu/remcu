cmake_minimum_required (VERSION 3.5)

# Write information to the console.
message("Running CMakeLists.txt for REMCU...")

set(CMAKE_VERBOSE_MAKEFILE ON)

# Maps to a solution file (Tutorial.sln). The solution will 
# have all targets (exe, lib, dll) as projects (.vcproj)
project(remcu-lib C CXX)

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
endif()

set(LIB_SOURCE ${SOURCE} ${NET_SOURCE})

set(TEST_IR_LL IrTest.ll)
set(TEST_IR_ADIN_LL IrTest.adin.ll)
set(TEST_IR_ADIN_LL_O IrTest.adin.ll.obj)

set(ADIN_OPT ${LLVM_ADIN_PATH}opt)

set(WIN_CLANG_IR_OPTIONS -D_MT -D_DLL)

add_custom_target (
  ${TEST_IR_LL}
  COMMAND clang -S -emit-llvm ${WIN_CLANG_IR_OPTIONS} ${CMAKE_CURRENT_SOURCE_DIR}/test/IrTest.cpp 
  -I${CMAKE_CURRENT_SOURCE_DIR}/export
  -I${CMAKE_CURRENT_SOURCE_DIR}/include
  -I${CMAKE_CURRENT_SOURCE_DIR}/test
  -o ${CMAKE_CURRENT_BINARY_DIR}/${TEST_IR_LL}
  )

add_custom_target (
  ${TEST_IR_ADIN_LL}
  COMMAND ${ADIN_OPT} -S -adin ${CMAKE_CURRENT_BINARY_DIR}/${TEST_IR_LL}
  -o ${CMAKE_CURRENT_BINARY_DIR}/${TEST_IR_ADIN_LL}
  DEPENDS ${TEST_IR_LL}
  )

add_custom_target (
  ${TEST_IR_ADIN_LL_O}
  COMMAND clang -c ${CMAKE_CURRENT_BINARY_DIR}/${TEST_IR_ADIN_LL}
  -o ${CMAKE_CURRENT_BINARY_DIR}/${TEST_IR_ADIN_LL_O}
  DEPENDS ${TEST_IR_ADIN_LL}
  )

SET_SOURCE_FILES_PROPERTIES(
  ${TEST_IR_ADIN_LL_O}
  PROPERTIES
  EXTERNAL_OBJECT true
  GENERATED true
  )

# shared library
add_library(remcu_dll SHARED ${LIB_SOURCE})
add_library(remcu_ir_dll SHARED ${LIB_SOURCE} ${TEST_IR_ADIN_LL_O})
	
add_executable(test_static test/test.cpp ${LIB_SOURCE} ${TEST_IR_ADIN_LL_O})
add_executable(test_dll test/test_dll.cpp)
target_link_libraries(test_dll remcu_ir_dll)

add_dependencies(test_static ${TEST_IR_ADIN_LL_O})

if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	target_link_libraries(remcu_dll ws2_32)
	target_link_libraries(remcu_ir_dll ws2_32)
	target_link_libraries(test_static ws2_32)
endif()