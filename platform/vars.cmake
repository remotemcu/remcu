
if(DEFINED ENV{LLVM_ADIN_PATH})
  set(LLVM_ADIN_PATH $ENV{LLVM_ADIN_PATH})
else()
  message(FATAL_ERROR "Not set LLVM_ADIN_PATH env var")
endif()

find_program(RESULT_FIND_OPT
  NAME opt
  PATHS ${LLVM_ADIN_PATH}
  NO_DEFAULT_PATH
  REQUIRED
  )

include(CMakePrintHelpers)
cmake_print_variables(RESULT_FIND_OPT)

if(NOT RESULT_FIND_OPT)
    message(FATAL_ERROR "opt binary not found at LLVM_ADIN_PATH path: ${LLVM_ADIN_PATH}")
endif()

add_custom_command(
  OUTPUT opt_file_find
  COMMAND ${RESULT_FIND_OPT} --version
  USES_TERMINAL
  DEPENDS RESULT_FIND_OPT)

exec_program(${RESULT_FIND_OPT}
  ARGS --version
  OUTPUT_VARIABLE LLVM_OPT_VERSION
  )


cmake_print_variables(LLVM_OPT_VERSION)

exec_program(clang
  ARGS --version
  OUTPUT_VARIABLE CLANG_VERSION
  )


cmake_print_variables(CLANG_VERSION)


set(REMCU_VM_PATH ${CMAKE_CURRENT_LIST_DIR}/../)
