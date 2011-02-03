/* 
* Copyright (C) 2007-2011 German Aerospace Center (DLR/SC)
*
* Created: 2010-08-13 Markus Litz <Markus.Litz@dlr.de>
* Changed: $Id: tixiUtils.h 4392 2011-02-03 08:52:55Z litz_ma $ 
*
* Version: $Revision: 4392 $
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
/**
 * @file   tixiUtils.h
 * @author Markus Litz <Markus.Litz@dlr.de>
 * @date   Tue May 28 12:06:33 2009
 *
 * @brief  Header file for routines used to implement the API.
 *
 * This file cotains declaration for utility routines used to implement the tixi API.
 */
#include "libxml/parser.h"
#include "libxml/xpath.h"
#include "libxml/xmlsave.h"
#include "tixi.h"
#include "tixiData.h"

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
int my_strncasecmp (const char *a, const char *b, size_t len);


/**
  @brief Creates a directory if not already exists.

 @param char dirname (in) The directory name to create
 @return
    - SUCCESS if successfully created
    - FAILED if directory could not be created
 */
int create_local_directory(const char *dirname);


/**
  @brief Checks if a string starts with substring .

 @param char dirname (in) The directory name to create
 @return
    - 0 if string starts with substring
    - -1 if string does not start with substring or an error occured
 */
int string_startsWith(char *string, char *subString);


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
ReturnCode strip_dirname(char *xmlFilename, char **dname, char **fname);


/**
  @brief Checks if a directory name seems to be relativ by checking if it starts with
  "file://". If the string starts with "file:///" (three slashes!), the path is absolute!

 @param char dirname (in) The directory name
 @return
    - 0 if path is realtiv
    - != 0 if path seems to be absolute
 */
int isPathRelative(char *dirname);


/**
  @brief Converts all chars of a string to lower case.

 @param char string (in) the string to be converted
 @return
    char lowerString (out) to string converted to lower case
 */
char* stringToLower(char* string);


/**
  @brief Strips the first len bytes of the string.
 @param char string (in)  The source string
 @param int len     (in)  The number of bytes to strip from s2
 @return
    char destinationString (out) to string converted to lower case
 */
char* string_stripLeft(char* string, int len);
