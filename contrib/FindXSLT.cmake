# - Find xslt
# Find the native XSLT headers and libraries.
#
#  XSLT_INCLUDE_DIRS   - where to find libxslt/xslt.h, etc.
#  XSLT_LIBRARIES      - List of libraries when using xslt.
#  XSLT_FOUND          - True if xslt found.
#  XSLT_VERSION_STRING - the version of xslt found (since CMake 2.8.8)

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
FIND_PATH(XSLT_INCLUDE_DIR NAMES libxslt/xslt.h)
MARK_AS_ADVANCED(XSLT_INCLUDE_DIR)

# Look for the library.
FIND_LIBRARY(XSLT_LIBRARY NAMES 
    xslt
  # Windows MSVC prebuilts:
	libxslt
)
MARK_AS_ADVANCED(XSLT_LIBRARY)

IF(XSLT_INCLUDE_DIR)
  FOREACH(_xslt_version_header xsltconfig.h xslt.h)
    IF(EXISTS "${XSLT_INCLUDE_DIR}/libxslt/${_xslt_version_header}")
      FILE(STRINGS "${XSLT_INCLUDE_DIR}/libxslt/${_xslt_version_header}" xslt_version_str REGEX "^#define[\t ]+LIBXSLT_DOTTED_VERSION[\t ]+\".*\"")

      STRING(REGEX REPLACE "^#define[\t ]+LIBXSLT_DOTTED_VERSION[\t ]+\"([^\"]*)\".*" "\\1" XSLT_VERSION_STRING "${xslt_version_str}")
      UNSET(xslt_version_str)
      BREAK()
    ENDIF()
  ENDFOREACH(_xslt_version_header)
ENDIF()

# handle the QUIETLY and REQUIRED arguments and set XSLT_FOUND to TRUE if 
# all listed variables are TRUE
#INCLUDE(${CMAKE_MODULE_PATH}/FindPackageHandleStandardArgs.cmake)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(XSLT
                                  REQUIRED_VARS XSLT_LIBRARY XSLT_INCLUDE_DIR
                                  VERSION_VAR XSLT_VERSION_STRING)

IF(XSLT_FOUND)
  SET(XSLT_LIBRARIES ${XSLT_LIBRARY})
  SET(XSLT_INCLUDE_DIRS ${XSLT_INCLUDE_DIR})
ENDIF(XSLT_FOUND)