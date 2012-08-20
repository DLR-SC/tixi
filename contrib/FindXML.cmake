# - Find xml
# Find the native XML headers and libraries.
#
#  XML_INCLUDE_DIRS   - where to find libxml/xml.h, etc.
#  XML_LIBRARIES      - List of libraries when using xml.
#  XML_FOUND          - True if xml found.
#  XML_VERSION_STRING - the version of xml found (since CMake 2.8.8)

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
FIND_PATH(XML_INCLUDE_DIR
    NAMES libxml/xmlversion.h
    PATH_SUFFIXES "libxml2"
)
MARK_AS_ADVANCED(XML_INCLUDE_DIR)

# Look for the library.
FIND_LIBRARY(XML_LIBRARY NAMES 
    xml2
  # Windows MSVC prebuilts:
	libxml2
)
MARK_AS_ADVANCED(XML_LIBRARY)

IF(XML_INCLUDE_DIR)
  FOREACH(_xml_version_header xmlversion)
    IF(EXISTS "${XML_INCLUDE_DIR}/libxml/${_xml_version_header}")
      FILE(STRINGS "${XML_INCLUDE_DIR}/libxml/${_xml_version_header}" xml_version_str REGEX "^#define[\t ]+LIBXML_DOTTED_VERSION[\t ]+\".*\"")

      STRING(REGEX REPLACE "^#define[\t ]+LIBXML_DOTTED_VERSION[\t ]+\"([^\"]*)\".*" "\\1" XML_VERSION_STRING "${xml_version_str}")
      UNSET(xml_version_str)
      BREAK()
    ENDIF()
  ENDFOREACH(_xml_version_header)
ENDIF()

# handle the QUIETLY and REQUIRED arguments and set XML_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(XML
                                  REQUIRED_VARS XML_LIBRARY XML_INCLUDE_DIR
                                  VERSION_VAR XML_VERSION_STRING)

IF(XML_FOUND)
  SET(XML_LIBRARIES ${XML_LIBRARY})
  SET(XML_INCLUDE_DIRS ${XML_INCLUDE_DIR})
ENDIF(XML_FOUND)
