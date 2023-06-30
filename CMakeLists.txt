cmake_minimum_required (VERSION 3.5)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
	message(error "not defined toolchain file")
endif()

include(GitDesc)
include(utils)
include(license)

# detect operating system and host processor
message(STATUS "We are on a ${CMAKE_SYSTEM_NAME} system")
message(STATUS "The host processor is ${CMAKE_HOST_SYSTEM_PROCESSOR}")

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SOURCE)

if(WIN32)
	set(NET_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/win/winsock2net)
else()
	aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/unix NET_SOURCE)
endif()

set(LIB_SOURCE ${SOURCE} ${NET_SOURCE})

add_library(remcu_object OBJECT ${LIB_SOURCE} ${CONF_SOURCE})

define_file_basename_for_sources(remcu_object)

target_compile_definitions(remcu_object PRIVATE _LICENSE_TYPE_=\"${LICENSE_TYPE}\")

target_include_directories(remcu_object PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(remcu_object PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/export)
target_include_directories(remcu_object PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if(WIN32)
  target_compile_definitions(remcu_object PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

set(LICENSE_SIGN_SCRIPT license.py)
set(LICENSE_SIGN_FILE REMCU_LICENSE.txt)

add_custom_target (
  ${LICENSE_SIGN_FILE}
  COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/${LICENSE_SIGN_SCRIPT}
  -int ${LICENSE_FILE_PATH}
  -out ${ARCHIVE_ROOT_PAT}/${LICENSE_SIGN_FILE}
  -t ${LICENSE_TYPE}
  )


add_dependencies(remcu_object ${LICENSE_SIGN_FILE})


file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/export/remcu.h
	DESTINATION ${ALL_INCLUDE_DIR}
)

execute_process(
	COMMAND ${CMAKE_COMMAND} -E touch 
		${ALL_INCLUDE_DIR}/remcu_exports_symbol_enter.h
		${ALL_INCLUDE_DIR}/remcu_exports_symbol_exit.h
		${ALL_INCLUDE_DIR}/__init__.py
	)



if(NOT DEFINED TARGET_REMCU_OS_NAME)
	if(NOT DEFINED ENV{TARGET_REMCU_OS_NAME})
		message(WARNING "******* $TARGET_REMCU_OS_NAME is not defined! ********")
		set(TARGET_REMCU_OS_NAME "undefined_os")
	else()
		set(TARGET_REMCU_OS_NAME $ENV{TARGET_REMCU_OS_NAME})
	endif()
endif()

if(DEFINED OPTION_DESCRIPTOR)
	message(WARNING "******* set option descriptor ${OPTION_DESCRIPTOR} ********")
else()
	set(OPTION_DESCRIPTOR C_INTERFACE)
endif()

get_git_describe(${CMAKE_CURRENT_SOURCE_DIR} GIT_DESCRIPTION)

get_git_describe(${CMAKE_SOURCE_DIR} GIT_MCU)

set(FULL_VERSION_NAME ${FULL_NAME_MCU_LIB}-${GIT_MCU}-${OPTION_DESCRIPTOR}-${TARGET_REMCU_OS_NAME}-${GIT_DESCRIPTION}-${LICENSE_TYPE})


target_compile_definitions(remcu_object PRIVATE _FULL_VERSION_NAME_=\"${FULL_VERSION_NAME}\")

if(CONSTEXPR_STRING_OBUSFICATION_ENABLE)
	target_compile_definitions(remcu_object PRIVATE CONSTEXPR_STRING_OBUSFICATION_ENABLE)
endif(CONSTEXPR_STRING_OBUSFICATION_ENABLE)


option(HIDE_SOURCE_INFO_IN_LOG "hide name of source file in log" OFF)
option(LICENSE_FILE_SKIP_ERROR "Skipping error of license file" ON)

if(HIDE_SOURCE_INFO_IN_LOG)
	target_compile_definitions(remcu_object PRIVATE HIDE_SOURCE_INFO_IN_LOG)
endif(HIDE_SOURCE_INFO_IN_LOG)

if(LICENSE_FILE_SKIP_ERROR)
	target_compile_definitions(remcu_object PRIVATE LICENSE_FILE_SKIP_ERROR)
endif(LICENSE_FILE_SKIP_ERROR)

if(CMAKE_SIZEOF_VOID_P EQUAL 8) # 64 bit
	target_compile_definitions(remcu_object PRIVATE FAST_CHECK_LOCAL_64BIT_ADDRESS)
endif()

set(FULL_ARCHIVE_NAME remcu-${FULL_VERSION_NAME} PARENT_SCOPE)

add_custom_target(PrintMessage ALL 
	COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --green 
			"*********** Final ${FULL_VERSION_NAME}  ****************")
