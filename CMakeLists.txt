cmake_minimum_required (VERSION 2.6)

# Set a default build type if none was specified
if(NOT DEFINED CMAKE_BUILD_TYPE)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    "MinSizeRel" "RelWithDebInfo")
endif()

project (TIXI)

OPTION(ENABLE_COVERAGE "Enable GCov coverage analysis (defines a 'coverage' target and enforces static build of tixi)" ON)

set(TIXI_VERSION_MAJOR 2)
set(TIXI_VERSION_MINOR 1)
set(TIXI_VERSION_PATCH 1pre)
set(TIXI_VERSION "${TIXI_VERSION_MAJOR}.${TIXI_VERSION_MINOR}.${TIXI_VERSION_PATCH}")

set(ADD_INCLUDE_PATH "" CACHE PATH "Additional include path for package search")
set(ADD_LIB_PATH "" CACHE PATH     "Additional library path for package search")

set(CMAKE_INCLUDE_PATH "${CMAKE_INCLUDE_PATH}" ${ADD_INCLUDE_PATH})
set(CMAKE_LIBRARY_PATH "${CMAKE_LIBRARY_PATH}" ${ADD_LIB_PATH})

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/contrib)

if(NOT DEFINED CMAKE_INSTALL_LIBDIR)
    set(CMAKE_INSTALL_LIBDIR "lib")
endif(NOT DEFINED CMAKE_INSTALL_LIBDIR)


if(NOT DEFINED CMAKE_INSTALL_BINDIR)
    set(CMAKE_INSTALL_BINDIR "bin")
endif(NOT DEFINED CMAKE_INSTALL_BINDIR)

find_package(CURL REQUIRED)
find_package(XSLT REQUIRED)
find_package(XML REQUIRED)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})

# code coverage analysis
IF (CMAKE_BUILD_TYPE STREQUAL "Debug")
        IF(ENABLE_COVERAGE)
                MESSAGE(STATUS "Coverage enabled")
                INCLUDE(CodeCoverage)
                SETUP_TARGET_FOR_COVERAGE(coverage ${PROJECT_NAME}-unittests coverageReport "--gtest_output=xml")
                SETUP_TARGET_FOR_COVERAGE_COBERTURA(coverage-cobertura ${PROJECT_NAME}-unittests coverage "--gtest_output=xml")
                MESSAGE(STATUS "Enabeling memcheck")
                INCLUDE(Valgrind)
                SETUP_TARGET_FOR_VALGRIND(memcheck ${PROJECT_NAME}-unittests valgrind)
        ELSE()
                MESSAGE(STATUS "Coverage disabled")
        ENDIF()
ENDIF()

# visual leak detector, useful for debugging under windows
if(WIN32)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        OPTION(TIXI_USE_VLD "Enable Visual Leak Detector." OFF)
        if(TIXI_USE_VLD)
            find_package( VLD REQUIRED )
            add_definitions(-DHAVE_VLD=1)
            include_directories(${VLD_INCLUDE_DIRS})
        endif(TIXI_USE_VLD)
    endif(CMAKE_BUILD_TYPE STREQUAL "Debug")
endif(WIN32)

configure_file(contrib/tixi-config-version.cmake.in 
    "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/tixi-config-version.cmake" @ONLY)
configure_file(contrib/tixi-config.cmake.in 
    "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/tixi-config.cmake" @ONLY)

configure_file(contrib/tixi-config-version.cmake.in 
    "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/tixi-config-version.cmake" @ONLY)
configure_file(contrib/tixi-config.cmake.in 
    "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/tixi-config.cmake" @ONLY)

#create tixi library
add_subdirectory(src)

#create bindings to different languages (fortran, java ...)
add_subdirectory(bindings)


#create gtests, override gtest standard setting
option(TIXI_BUILD_TESTS "Build TIXI Testsuite" OFF)
if(TIXI_BUILD_TESTS)
  option(gtest_force_shared_crt "" ON)
  mark_as_advanced(gtest_force_shared_crt)
  set (gtest_SOURCE_DIR "thirdparty/gtest-1.6.0")
  add_subdirectory (${gtest_SOURCE_DIR})
  add_subdirectory(tests)
endif(TIXI_BUILD_TESTS)

#demos
add_subdirectory(examples/Demo)

# create the doc
include(createDoc)

INCLUDE(InstallRequiredSystemLibraries)

set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Martin Siggel") #required for debian/ubuntu
set(CPACK_PACKAGE_VENDOR "www.dlr.de/sc")
set(CPACK_PACKAGE_VERSION ${TIXI_VERSION})
set(CPACK_PACKAGE_VERSION_MAJOR ${TIXI_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${TIXI_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${TIXI_VERSION_PATCH})
set(CPACK_RESOURCE_FILE_LICENSE ${PROJECT_SOURCE_DIR}/LICENSE)
set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "TIXI") 

# set path variable for installer
set(CPACK_NSIS_MODIFY_PATH ON)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES64")
  set(CPACK_CUSTOM_INITIAL_DEFINITIONS "!define CPACK_REQUIRIRE_64BIT")
else()
  set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES")
endif()

include(CPack)
cpack_add_component(Runtime DISPLAY_NAME "3rd Party Libraries")
cpack_add_component(headers DISPLAY_NAME "Headers")
cpack_add_component(interfaces DISPLAY_NAME "Interfaces/Bindings")
cpack_add_component(docu DISPLAY_NAME "Documentation")
