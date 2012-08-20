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

set(TIXI_VERSION "2.0")

set(ADD_INCLUDE_PATH "" CACHE PATH "Additional include path for package search")
set(ADD_LIB_PATH "" CACHE PATH     "Additional library path for package search")

set(CMAKE_INCLUDE_PATH "${CMAKE_INCLUDE_PATH}" ${ADD_INCLUDE_PATH})
set(CMAKE_LIBRARY_PATH "${CMAKE_LIBRARY_PATH}" ${ADD_LIB_PATH})

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/contrib)

find_package(CURL REQUIRED)
find_package(XSLT REQUIRED)
find_package(XML REQUIRED)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

#create tixi library
add_subdirectory(src)

#create bindings to different languages (fortran, java ...)
add_subdirectory(bindings)

#create tests
add_subdirectory(tests)

#demos
add_subdirectory(examples/Demo)

# create the doc
include(createDoc)

