
if(NOT LICENSE_TYPE)
	if(NOT ENV{LICENSE_TYPE})
		message(FATAL_ERROR "******* $LICENSE_TYPE is not defined! ********")
	else()
		set(LICENSE_TYPE ENV{LICENSE_TYPE})
	endif()
endif()


if(LICENSE_TYPE STREQUAL "EDU")
	message( "******* EDUCATION license ********")
elseif(LICENSE_TYPE STREQUAL "COM")
	message( "******* COMMERCIAL license ********")
else()
	message(FATAL_ERROR "******* unknown license '${LICENSE_TYPE}' ********")
endif()

if(NOT LICENSE_FILE_PATH)
	if(NOT ENV{LICENSE_FILE_PATH})
		set(LICENSE_FILE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/license_text/${LICENSE_TYPE}_LICENSE.txt)
	else()
		set(LICENSE_FILE_PATH ENV{LICENSE_FILE_PATH})
	endif()
endif()


add_definitions(-DLICENSE_TYPE=\"${LICENSE_TYPE}\")

set(LICENSE_SIGN_SCRIPT license.py)
set(LICENSE_SIGN_FILE REMCU_LICENSE.txt)

add_custom_target (
  ${LICENSE_SIGN_FILE}
  COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/${LICENSE_SIGN_SCRIPT}
  -int ${LICENSE_FILE_PATH}
  -out ${CMAKE_CURRENT_BINARY_DIR}/${LICENSE_SIGN_FILE}
  -t ${LICENSE_TYPE}
  )