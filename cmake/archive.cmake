
if(WIN32)
	set(ARCHIVE_COMMAND zip -r ${FULL_ARCHIVE_NAME}.zip)
else()
	set(ARCHIVE_COMMAND tar -cf ${FULL_ARCHIVE_NAME}.tar)
endif()