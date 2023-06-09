
if(DEFINED REMCU_PREBUILD_CONFIG_FILE)
      include(REMCU_PREBUILD_CONFIG_FILE)
endif()

set(BUILD_IR_TEST_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/IrTest)

set(IR_TEST_OBJECT_FILE ${BUILD_IR_TEST_DIRECTORY}/IrTest.adin.o)


include(adin_make_flags)

add_custom_target( IR_TEST_OBJ
                   COMMAND make all
                   ${IR_TEST_MAKE_ARGUMENTS}
                   OUTPUT=${IR_TEST_OBJECT_FILE}
                   BUILD_DIRECTORY=${BUILD_IR_TEST_DIRECTORY}
                   WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
                   COMMENT "---------------- Generating IR Test ----------------"
)
