SET_SOURCE_FILES_PROPERTIES(
  ${MCU_OBJECT_FILE}
  PROPERTIES
  EXTERNAL_OBJECT true
  GENERATED true
  )

add_library(remcu SHARED ${MCU_OBJECT_FILE} $<TARGET_OBJECTS:remcu_object>)

add_dependencies(remcu MCU_LIB)
add_dependencies(remcu remcu_object)

set(ARCHIVE_FILES $<TARGET_FILE_NAME:remcu> 
  ${CMAKE_CURRENT_BINARY_DIR}/REMCU_LICENSE.txt ${ALL_INCLUDE_DIR} 
  ${CMAKE_CURRENT_BINARY_DIR}/README.txt)

file(INSTALL ${CMAKE_CURRENT_SOURCE_DIR}/README.txt
      DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
      )

add_custom_command(TARGET remcu POST_BUILD
    COMMAND strip --strip-all $<TARGET_FILE_NAME:remcu>
    COMMAND ${CMAKE_COMMAND} -E tar cf ${FULL_ARCHIVE_NAME}.${ARCHIVE_SUFFIX} --format=${ARCHIVE_FORMAT} --
    ${ARCHIVE_FILES}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "--------------------------- Archive lib ---------------------------"
)
