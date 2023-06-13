
if(DEFINED ENV{LLVM_ADIN_PATH})
  set(LLVM_ADIN_PATH $ENV{LLVM_ADIN_PATH})
else()
  message(FATAL_ERROR "Not set LLVM_ADIN_PATH env var")
endif()

include(CMakePrintHelpers)
cmake_print_variables(CMAKE_CURRENT_LIST_DIR)


set(REMCU_VM_PATH ${CMAKE_CURRENT_LIST_DIR}/../)