
set(CMAKE_CXX_STANDARD 14)

set(STRIP_ARGUMENT --strip-all)

set(CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE} "-O2 -DNDEBUG" # -fvisibility=hidden -s"
 CACHE STRING "Linux x64 CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}")

set(CMAKE_CXX_FLAGS  ${CMAKE_CXX_FLAGS} "-fno-rtti -fno-exceptions -fPIC " 
	CACHE STRING "Linux x64 CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")


# Configure CCache if available
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
        set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
        set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)