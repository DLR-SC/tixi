# TIXI Library
# @author Martin Siggel

configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/tixi_version.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/tixi_version.h"
)

set(TIXI_SRC tixiImpl.c tixiUtils.c uidHelper.c tixiInternal.c webMethods.c 
	xpathFunctions.c xslTransformation.c)

set(TIXI_HDR tixi.h tixiData.h tixiInternal.h tixiUtils.h)


include_directories(${CURL_INCLUDE_DIRS} ${XML_INCLUDE_DIRS} ${XSLT_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_definitions(-DDEBUG) 
endif()

if(CMAKE_COMPILER_IS_GNUCC)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -fmessage-length=0")
endif()

set(TIXI_LIBS ${CURL_LIBRARIES} ${XSLT_LIBRARIES} ${XML_LIBRARIES} ${VLD_LIBRARIES})
add_library(TIXI SHARED ${TIXI_SRC} ${TIXI_HDR})
target_link_libraries (TIXI  ${TIXI_LIBS})
set_target_properties( TIXI PROPERTIES VERSION ${TIXI_VERSION})
set_target_properties( TIXI PROPERTIES SOVERSION "${TIXI_VERSION_MAJOR}")

add_library(TIXI_static ${TIXI_SRC} ${TIXI_HDR})
target_link_libraries (TIXI_static  ${TIXI_LIBS})

install (TARGETS TIXI 
		 EXPORT tixi-targets
		 RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
		 LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
		 ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
		 )
		 
install (FILES tixi.h ${CMAKE_CURRENT_BINARY_DIR}/tixi_version.h
		 DESTINATION include
		 COMPONENT headers)
		 
install (EXPORT tixi-targets DESTINATION ${CMAKE_INSTALL_LIBDIR}/tixi)	 

install (FILES
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/tixi-config.cmake"
  "${PROJECT_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/tixi-config-version.cmake"
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/tixi)
