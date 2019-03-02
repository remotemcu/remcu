
if(WIN32)
	set(ARCHIVE_COMMAND zip -r ${FULL_VERSION_NAME}.zip)
else()
	set(ARCHIVE_COMMAND tar -cf ${FULL_VERSION_NAME}.tar)
endif()