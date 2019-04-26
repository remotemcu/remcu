cmake_minimum_required (VERSION 3.10)

find_package(GTest REQUIRED)
include(GoogleTest)

function(gtest_wrap TARGET SOURCES)
    SET_SOURCE_FILES_PROPERTIES(
      ${IR_TEST_OBJECT_FILE}
      PROPERTIES
      EXTERNAL_OBJECT true
      GENERATED true
      )
    add_executable(${TARGET} ${SOURCES}  $<TARGET_OBJECTS:remcu_object> ${IR_TEST_OBJECT_FILE})
    add_dependencies(${TARGET} remcu_object)
    add_dependencies(${TARGET} IR_TEST_OBJ)
    target_link_libraries(${TARGET} GTest::GTest)
    target_include_directories(${TARGET} PRIVATE ${REMCU_VM_PATH}/include)
    target_include_directories(${TARGET} PRIVATE ${REMCU_VM_PATH}/export)

    gtest_discover_tests(${TARGET})

endfunction()

gtest_wrap(gdb_test gdb_client.cpp)

#[[

add_executable(gdb_test gdb_client.cpp  $<TARGET_OBJECTS:remcu_object> ${IR_TEST_OBJECT_FILE})
add_dependencies(gdb_test remcu_object)
add_dependencies(gdb_test IR_TEST_OBJ)

target_link_libraries(gdb_test GTest::GTest)

gtest_discover_tests(gdb_test)

target_include_directories(gdb_test PRIVATE ${REMCU_VM_PATH}/include)
target_include_directories(gdb_test PUBLIC ${REMCU_VM_PATH}/export)
]]
#gtest_add_tests(gdb_test "" AUTO)
#add_test(NAME gdb_test COMMAND gdb_test)

#gtest_discover_tests(gdb_test "" AUTO)

#add_test(NAME monolithic COMMAND gdb_test)

#add_test(monolithic gdb_test)
