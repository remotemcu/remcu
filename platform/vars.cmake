
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


set(REMCU_VM_PATH ${CMAKE_CURRENT_LIST_DIR}/../)
