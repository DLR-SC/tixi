# - Find curl
# Find the native CURL headers and libraries.
#
#  CURL_INCLUDE_DIRS   - where to find curl/curl.h, etc.
#  CURL_LIBRARIES      - List of libraries when using curl.
#  CURL_FOUND          - True if curl found.
#  CURL_VERSION_STRING - the version of curl found (since CMake 2.8.8)
#
# Creates the CMake target CURL::libcurl for the curl library

#=============================================================================
# Copyright 2006-2009 Kitware, Inc.
# Copyright 2012 Rolf Eike Beer <eike@sf-mail.de>
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

# Look for the header file.
find_path(CURL_INCLUDE_DIR NAMES curl/curl.h)
mark_as_advanced(CURL_INCLUDE_DIR)

# Look for the library (sorted from most current/relevant entry to least).
find_library(CURL_LIBRARY NAMES
    curl
  # Windows MSVC prebuilts:
    curllib
    libcurl_imp
    curllib_static
    libcurl_a
  # Windows older "Win32 - MSVC" prebuilts (libcurl.lib, e.g. libcurl-7.15.5-win32-msvc.zip):
    libcurl
)
mark_as_advanced(CURL_LIBRARY)

if(CURL_INCLUDE_DIR)
  foreach(_curl_version_header curlver.h curl.h)
    if(EXISTS "${CURL_INCLUDE_DIR}/curl/${_curl_version_header}")
      file(STRINGS "${CURL_INCLUDE_DIR}/curl/${_curl_version_header}" curl_version_str REGEX "^#define[\t ]+LIBCURL_VERSION[\t ]+\".*\"")

      string(REGEX REPLACE "^#define[\t ]+LIBCURL_VERSION[\t ]+\"([^\"]*)\".*" "\\1" CURL_VERSION_STRING "${curl_version_str}")
      unset(curl_version_str)
      break()
    endif()
  endforeach()
endif()

# handle the QUIETLY and REQUIRED arguments and set CURL_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(CURL
                                  REQUIRED_VARS CURL_LIBRARY CURL_INCLUDE_DIR
                                  VERSION_VAR CURL_VERSION_STRING)

if(CURL_FOUND)
  add_library(CURL::libcurl UNKNOWN IMPORTED)
  set_target_properties(CURL::libcurl PROPERTIES
      INTERFACE_INCLUDE_DIRECTORIES ${CURL_INCLUDE_DIR}
      IMPORTED_LOCATION ${CURL_LIBRARY}
      IMPORTED_IMPLIB ${CURL_LIBRARY}
  )
  if (WIN32)
    string(REGEX MATCH "_a.lib$" CURL_STATICLIB ${CURL_LIBRARY})
    if (CURL_STATICLIB)
      set_target_properties(CURL::libcurl
          PROPERTIES INTERFACE_COMPILE_DEFINITIONS "CURL_STATICLIB"
      )
    endif(CURL_STATICLIB)
  endif (WIN32)
endif()
