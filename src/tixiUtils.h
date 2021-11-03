/*
* Copyright (C) 2015 German Aerospace Center (DLR/SC)
*
* Created: 2010-08-13 Markus Litz <Markus.Litz@dlr.de>
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*   http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#ifndef TIXI_UTILS_H
#define TIXI_UTILS_H

#include "libxml/parser.h"
#include "libxml/xpath.h"
#include "libxml/xmlsave.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file   tixiUtils.h
 * @author Markus Litz <Markus.Litz@dlr.de>
 * @date   Tue May 28 12:06:33 2009
 *
 * @brief  Header file for routines used to implement the API.
 *
 * This file cotains declaration for utility routines used to implement the tixi API.
 */

#include "tixi.h"
#include "tixiData.h"
#include "tixiInternal.h"

/**
  @brief compares up to <[length]> characters
  from the string at <[a]> to the string at <[b]> in a
  case-insensitive manner.


 @param char dirname (in) The directory name to create
 @return
    - 0 if bot string are equivalent
    - !=0 If <<*<[a]>>> sorts lexicographically before <<*<[b]>>>,
          <<strncasecmp>> returns a number less than zero
 */
TIXI_INTERNAL_EXPORT int my_strncasecmp (const char *a, const char *b, size_t len);


/**
  @brief Creates a directory if not already exists.

  @param char dirname (in) The directory name to create
  @return
    - SUCCESS if successfully created
    - FAILED if directory could not be created
 */
TIXI_INTERNAL_EXPORT int create_local_directory(const char *dirname);


/**
  @brief Checks if a string starts with substring .

  @return
    - 0 if string starts with substring
    - -1 if string does not start with substring or an error occured
 */
TIXI_INTERNAL_EXPORT int string_startsWith(const char *string, const char *subString);

/**
  @brief Checks if a string ends with substring .

  @return
    - 0 if string ends with substring
    - -1 if string does not end with substring or an error occured
 */
TIXI_INTERNAL_EXPORT int string_endsWith(const char *string, const char *subString);

/**
 * @brief Creates a substring. The user has to free it
 * @param str The input string
 * @param start_pos Starting position.
 * @param end_pos End position. -1 indicates the end of the string.
 * @return
 */
TIXI_INTERNAL_EXPORT char* substring(const char* str, int start_pos, int end_pos);

/**
  @brief Returns the directory- and filename. The dirname will always end
  with a "/" seperator! Memory for the string will be allocated in this
  function and has to be freed manualy.

  @param char* xmlFilname (in) The full filename with path.
  @param char** dname (out) The directory name of the xmlFilename
  @param char** fname (out) The file name of the xmlFilename
  @return
    - SUCCESS
    - FAILED internal error
 */
TIXI_INTERNAL_EXPORT ReturnCode strip_dirname(const char *xmlFilename, char **dname, char **fname);


/**
  @brief Checks if a directory name seems to be relativ by checking if it starts with
  "file://". If the string starts with "file:///" (three slashes!), the path is absolute!

  @param char dirname (in) The directory name
  @return
    - 0 if path is realtiv
    - != 0 if path seems to be absolute
 */
TIXI_INTERNAL_EXPORT int isPathRelative(const char *dirname);



/**
  @brief Checks if a local directory is relative or absolute.

  @param char dirname (in) The directory name
  @return
    - 0 if path is realtive
    - != 0 if path seems to be absolute
 */
TIXI_INTERNAL_EXPORT int isLocalPathRelative(const char *dirname);

/**
  @brief Checks if a path is a URI (instead of a local path)
  @param path The file/directory path
  @return
    - 0, if is URI
    - != 0, if local path
 */
TIXI_INTERNAL_EXPORT int isURIPath(const char* path);

/**
  @brief uriToLocalPath converts a local URI (e.g. file://data.txt) to a local path (data.txt)
  @param uri URI to convert
  @return
    local Path or zero, if the URI is a remote path and can't be converted
 */
TIXI_INTERNAL_EXPORT char* uriToLocalPath(const char* uri);

/**
  @brief localPathToURI converts a local path (e.g. ./data.txt) to an URI (file://data.txt)
  @param path Path to convert
  @return
    The URI of the path
 */
TIXI_INTERNAL_EXPORT char* localPathToURI(const char* path);


/**
  @brief loadFileToString Loads a file from disk and returns it as a string.
  @param path The local path to the file (no URI). E.g. /data/myfile.txt. The path may be relative as well.
  @return
    The file as a string or a NULL pointer, if the file could not be loaded.
 */
TIXI_INTERNAL_EXPORT char* loadFileToString(const char* path);

/**
  @brief Converts all chars of a string to lower case.

  @param char string (in) the string to be converted
  @return
    string converted to lower case
 */
TIXI_INTERNAL_EXPORT char* stringToLower(char* string);

/**
  @brief tests, wether a given string is numeric
  @return
    1, if the string can be converted to a number and 0 else.
 */
TIXI_INTERNAL_EXPORT int isNumeric (const char * s);

/**
  @brief strips any trailing whitespace from a string in place
  @param char str (in) The source string
 */
TIXI_INTERNAL_EXPORT void trim_trailing_whitespace(char * str);

/**
  @brief Strips the first len bytes of the string.
  @param char string (in)  The source string
  @param int n       (in)  The number of bytes to strip from s2
  @return
    a string with n characters removed from the string
 */
TIXI_INTERNAL_EXPORT char* string_stripLeft(const char *string, int n);

/**
  @brief If inDirectory is a relative path,
  the function perpends the current working directory.

  E.g. if inDirectory is data/TestData and the working directory
  is ./tests/, the function returns file://./tests/data/TestData/

  If inDirectory is an absolute path, the working directory
  is not prepended.

  Also, this function converts the path into a uri, i.e.
  it prepends 'file://'

  @param The working directory must end with a "/"

  @return The corrected path
 */
TIXI_INTERNAL_EXPORT char* resolveDirectory(const char* workingDirectory, const char* inDirectory);

#ifdef __cplusplus
} /* extern C */
#endif

#endif /* TIXI_UTILS_H */
