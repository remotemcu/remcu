
if(NOT LICENSE_TYPE)
	if(NOT ENV{LICENSE_TYPE})
		message(FATAL_ERROR "******* $LICENSE_TYPE is not defined! ********")
	else()
		set(LICENSE_TYPE ENV{LICENSE_TYPE})
	endif()
endif()


# Strip lead and trailing whitepasce
#string(STRIP ${LICENSE_TYPE} LICENSE_TYPE)


if(LICENSE_TYPE STREQUAL "EDU")
	message( "******* EDUCATION license ********")
elseif(LICENSE_TYPE STREQUAL "COM")
	message( "******* COMMERCIAL license ********")
else()
	message(FATAL_ERROR "******* unknown license '${LICENSE_TYPE}' ********")
endif()

add_definitions(-DLICENSE_TYPE=\"${LICENSE_TYPE}\" )