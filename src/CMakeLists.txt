# TIXI Library
# @author Martin Siggel

find_package(CURL REQUIRED)
find_package(LibXml2 REQUIRED)
find_package(LibXslt REQUIRED)

configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/tixi_version.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/tixi_version.h"
)

set(TIXI_SRC tixiImpl.c tixiUtils.c uidHelper.c tixiInternal.c webMethods.c 
    xpathFunctions.c xslTransformation.c namespaceFunctions.c)

set(TIXI_HDR tixi.h tixiData.h tixiInternal.h tixiUtils.h)


if(CMAKE_COMPILER_IS_GNUCC)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -Wall -fmessage-length=0")
endif()

set(TIXI_LIBS curllib LibXslt::LibXslt LibXml2::LibXml2)
if(WIN32)
    set(TIXI_LIBS ${TIXI_LIBS} Shlwapi)
endif(WIN32)

if(APPLE)
    set(TIXI_LIBS ${TIXI_LIBS} m "-framework Foundation" iconv z "-framework Security")
endif(APPLE)

add_library(${TIXI_LIB_NAME} ${TIXI_SRC} ${TIXI_HDR})

target_compile_definitions(${TIXI_LIB_NAME} PRIVATE $<$<CONFIG:DEBUG>:DEBUG>)

if(WIN32)
    target_compile_definitions(${TIXI_LIB_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)
endif(WIN32)

target_include_directories(${TIXI_LIB_NAME} PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDE_DIR}>
)

target_link_libraries (${TIXI_LIB_NAME} PRIVATE ${TIXI_LIBS})
set_target_properties(${TIXI_LIB_NAME} PROPERTIES VERSION ${TIXI_VERSION_MINOR}.${TIXI_VERSION_PATCH})
set_target_properties(${TIXI_LIB_NAME} PROPERTIES SOVERSION "${TIXI_VERSION_MINOR}")

add_library(tixi_internal_api INTERFACE)
target_link_libraries(tixi_internal_api INTERFACE ${TIXI_LIBS})

target_include_directories(tixi_internal_api INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)


set(CMAKE_INSTALL_INCLUDE_DIR "include/${TIXI_LIB_NAME}")

install (TARGETS ${TIXI_LIB_NAME} EXPORT tixi3-targets
         RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
         LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
         ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
         
install (FILES tixi.h ${CMAKE_CURRENT_BINARY_DIR}/tixi_version.h
         DESTINATION ${CMAKE_INSTALL_INCLUDE_DIR}
         COMPONENT headers)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/tixi3-config-version.cmake"
  VERSION ${TIXI_VERSION}
  COMPATIBILITY AnyNewerVersion
)

file(GENERATE
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/tixi3-config.cmake"
  INPUT  "${PROJECT_SOURCE_DIR}/cmake/tixi-config.cmake.in"
)

install (EXPORT tixi3-targets DESTINATION ${CONFIG_INSTALL_DIR})

install (FILES
    "${CMAKE_CURRENT_BINARY_DIR}/tixi3-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/tixi3-config-version.cmake"
    DESTINATION ${CONFIG_INSTALL_DIR})
