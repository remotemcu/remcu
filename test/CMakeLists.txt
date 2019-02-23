
if(WIN32)
  set(CLANG_IR_OPTIONS -D_MT -D_DLL)
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("WIN debug option: -D_DEBUG")
    list(APPEND CLANG_IR_OPTIONS -D_DEBUG)
  endif()
else()
  set(CLANG_IR2O_OPTIONS -fPIC)
endif()

set(TEST_IR_LL IrTest.ll)
set(TEST_IR_ADIN_LL IrTest.adin.ll)
set(TEST_IR_ADIN_LL_O IrTest.adin.ll.obj)

add_custom_target (
  ${TEST_IR_LL}
  COMMAND clang -S -emit-llvm ${CLANG_IR_OPTIONS} ${CMAKE_CURRENT_SOURCE_DIR}/IrTest.cpp 
  -I${CMAKE_CURRENT_SOURCE_DIR}/../export
  -I${CMAKE_CURRENT_SOURCE_DIR}/../include
  -I${CMAKE_CURRENT_SOURCE_DIR}/
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
  ${CLANG_IR2O_OPTIONS}
  -o ${CMAKE_CURRENT_BINARY_DIR}/${TEST_IR_ADIN_LL_O}
  DEPENDS ${TEST_IR_ADIN_LL}
  )

SET_SOURCE_FILES_PROPERTIES(
  ${TEST_IR_ADIN_LL_O}
  PROPERTIES
  EXTERNAL_OBJECT true
  GENERATED true
  )


add_library(remcu_ir_shared SHARED ${LIB_SOURCE} ${TEST_IR_ADIN_LL_O})
add_executable(test_static test.cpp ${LIB_SOURCE} ${TEST_IR_ADIN_LL_O})
add_executable(test_shared test_shared.cpp)
target_link_libraries(test_shared remcu_ir_shared)

define_file_basename_for_sources(remcu_ir_shared)
define_file_basename_for_sources(test_static)
define_file_basename_for_sources(test_shared)

add_dependencies(test_static ${TEST_IR_ADIN_LL_O})
add_dependencies(remcu_ir_shared ${TEST_IR_ADIN_LL_O})

if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
  target_link_libraries(test_static ws2_32)
  target_link_libraries(remcu_ir_shared ws2_32)
endif()