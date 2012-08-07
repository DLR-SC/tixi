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
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
/**
   @file Utility functions used to fetch files from a url.
*/

#define _CRT_SECURE_NO_WARNINGS
#include "webMethods.h"

#include <curl/curl.h>
#include <curl/easy.h>



void *myrealloc(void *ptr, size_t size)
{
	/* There might be a realloc() out there that doesn't like reallocing
     NULL pointers, so we take care of it here */
    if (ptr)
        return realloc(ptr, size);
    else
        return malloc(size);
}

size_t writeMemoryCallback(void *ptr, size_t size, size_t nmemb, void *data)
{
	size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *) data;

    mem->memory = (char *) myrealloc(mem->memory, mem->size + realsize + 1);
    if (mem->memory) {
        memcpy(&(mem->memory[mem->size]), ptr, realsize);
        mem->size += realsize;
        mem->memory[mem->size] = 0;
    }
    return realsize;
}

char* curlGetURLInMemory(const char *url)
{
    CURL *curl_handle;
    CURLcode res;
    char curl_error_buffer[CURL_ERROR_SIZE];

    struct MemoryStruct chunk;

    chunk.memory = NULL; /* we expect realloc(NULL, size) to work */
    chunk.size = 0; /* no data at this point */

    curl_global_init(CURL_GLOBAL_ALL);

    /* init the curl session */
    curl_handle = curl_easy_init();

    /* specify URL to get */
    curl_easy_setopt(curl_handle, CURLOPT_URL, url);

    /* send all data to this function  */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, writeMemoryCallback);

    /* we pass our 'chunk' struct to the callback function */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);

    /* For error reasons set error puffer */
    curl_easy_setopt(curl_handle, CURLOPT_ERRORBUFFER, &curl_error_buffer);

    /* some servers don't like requests that are made without a user-agent
     field, so we provide one */
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0-TIXI");

    /* get it! */
    res = curl_easy_perform(curl_handle);

    /* cleanup curl stuff */
    curl_easy_cleanup(curl_handle);

    if (res != CURLE_OK)
    {
        fprintf(stderr, "curl request failed: %s", curl_error_buffer);
        return NULL;
    }

    return (chunk.memory);
}


int curlGetFileToLocalDisk(const char* url, const char* local)
{
	CURL *curl;
    CURLcode res;
    FILE *localFile;

    /* local file name to store the file as */
    localFile = fopen(local, "wb"); /* b is binary for win */

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_FILE, localFile);
        res = curl_easy_perform(curl);

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    fclose(localFile); /* close the local file */
    return 0;
}
