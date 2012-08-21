# TIXI Library
# @author Martin Siggel

set(TIXI_SRC tixiImpl.c tixiUtils.c uidHelper.c tixiInternal.c webMethods.c 
	xpathFunctions.c xslTransformation.c)

set(TIXI_HDR tixi.h tixiData.h tixiInternal.h tixiUtils.h)


include_directories(${CURL_INCLUDE_DIRS} ${XML_INCLUDE_DIRS} ${XSLT_INCLUDE_DIRS})

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_definitions(-DDEBUG) 
endif()

if(CMAKE_COMPILER_IS_GNUCC)
	set(CMAKE_C_FLAGS "-Wall -fmessage-length=0")
endif()

add_library(TIXI SHARED ${TIXI_SRC} ${TIXI_HDR})
target_link_libraries (TIXI  ${CURL_LIBRARIES} ${XSLT_LIBRARIES} ${XML_LIBRARIES})
set_target_properties( TIXI PROPERTIES VERSION ${TIXI_VERSION})

install (TARGETS TIXI 
		 DESTINATION lib
		 COMPONENT libraries)
install (FILES tixi.h 
		 DESTINATION include
		 COMPONENT headers)