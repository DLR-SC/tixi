/* 
* Copyright (C) 2007-2011 German Aerospace Center (DLR/SC)
*
* Created: 2010-08-13 Markus Litz <Markus.Litz@dlr.de>
* Changed: $Id$ 
*
* Version: $Revision$
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
/**
 * @file   webMethods.h
 * @author Markus Litz <Markus.Litz@dlr.de>
 * @date   Tue May 28 12:06:33 2009
 * @brief Utility functions used to fetch files from a url.
 * @file Utility functions used to fetch files from a url.
 */
#ifndef WEB_METHODS_H
#define WEB_METHODS_H

#include "libxml/parser.h"
#include "libxml/xpath.h"
#include "libxml/xmlsave.h"
#include "tixi.h"
#include "tixiData.h"

#ifdef __cplusplus
extern "C" {
#endif

struct MemoryStruct{
	char *memory;
    size_t size;
};


/**
  @brief Simple download a URL source and save it to a local file.

 @param const char* url   (in) The source (http) URL of the resource
 @param const char* local (in) The destination file on the local harddrive
 @return int
	- 0 if SUCCESS
	- != 0 if ERROR
 */
int curlGetFileToLocalDisk(const char* url, const char* local);


/**
  @brief Reallocating of memory, taking care of null pointers.

 @param void* ptr    (in) The memory pointer
 @param size_t size	 (in) The (new) size
 @return void
 */
void *myrealloc(void *ptr, size_t size);


/**
  @brief Write callback needed for curl.

 @param void* ptr    (in) The memory pointer
 @param size_t size	 (in) The size
 @param size_t nmemb (in) Byte size
 @param void* data   (in) The data to copy
 @return size_t
	 - The real copies size in byte.
 */
size_t writeMemoryCallback(void *ptr, size_t size, size_t nmemb, void *data);


/**
  @brief Simple download a URL source into local memory.

 @param const char* url   (in) The source (http) URL of the resource
 @return char*
	The content of the url-resource as string.
 */
char* curlGetURLInMemory(const char *url);

#ifdef __cplusplus
}
#endif

#endif /* WEB_METHODS_H */
