SET_SOURCE_FILES_PROPERTIES(
  ${MCU_OBJECT_FILE}
  PROPERTIES
  EXTERNAL_OBJECT true
  GENERATED true
  )


include(${REMCU_VM_PATH}/ir_test/IrTest.cmake)

SET_SOURCE_FILES_PROPERTIES(
  ${IR_TEST_OBJECT_FILE}
  PROPERTIES
  EXTERNAL_OBJECT true
  GENERATED true
  )

add_library(remcu SHARED ${MCU_OBJECT_FILE} ${IR_TEST_OBJECT_FILE} $<TARGET_OBJECTS:remcu_object>)

add_dependencies(remcu MCU_LIB)
add_dependencies(remcu IR_TEST_OBJ)
add_dependencies(remcu remcu_object)

list(APPEND ARCHIVE_FILES $<TARGET_FILE_NAME:remcu> 
  ${CMAKE_CURRENT_BINARY_DIR}/REMCU_LICENSE.txt ${ALL_INCLUDE_DIR} 
  ${CMAKE_CURRENT_BINARY_DIR}/README.txt)

if(WIN32)
  list(APPEND ARCHIVE_FILES ${CMAKE_CURRENT_BINARY_DIR}/remcu.lib)
endif()

file(INSTALL ${REMCU_VM_PATH}/README.txt
      DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
      )


if(WIN32)
  set(ARCHIVE_FORMAT zip)
  set(ARCHIVE_SUFFIX zip)
else()
  set(ARCHIVE_FORMAT gnutar)
  set(ARCHIVE_SUFFIX tar)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Release")
  add_custom_command(TARGET remcu POST_BUILD
# if need strip, uncomment
#      COMMAND ${STRIP_COMMAND} ${STRIP_ARGUMENT} $<TARGET_FILE_NAME:remcu>
      COMMAND ${CMAKE_COMMAND} -E tar cvf ${FULL_ARCHIVE_NAME}.${ARCHIVE_SUFFIX} --format=${ARCHIVE_FORMAT} --
      ${ARCHIVE_FILES}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "--------------------------- Archive lib ---------------------------"
  )
endif()


install(TARGETS remcu DESTINATION ${DESTINATION_DIR_NAME})
install(DIRECTORY ${ALL_INCLUDE_DIR} 
    DESTINATION ${DESTINATION_DIR_NAME})

if(WIN32)
  #target_compile_definitions(remcu PRIVATE _CRT_SECURE_NO_WARNINGS)
  target_link_libraries(remcu ws2_32)
  if(NOT MSVC)
    target_link_libraries(remcu msvcrtd)
  endif(NOT MSVC)
endif()
