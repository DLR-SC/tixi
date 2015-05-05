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
   @file Utility functions used to implement the interface.
*/
#define _CRT_SECURE_NO_WARNINGS
#include "tixiUtils.h"
#include "tixiInternal.h"

#include <ctype.h>
#ifdef _WIN32
  #include <direct.h>
  #include <Shlwapi.h>
  #define mkdir(dir, attr) _mkdir((dir))
  #define rmdir(dir) _rmdir((dir))
#else
  #include <sys/stat.h>
  #include <unistd.h>
#endif

extern TixiPrintMsgFnc printMsg;

int my_strncasecmp (const char *a, const char *b, size_t len)
{
  register int n;

  if (len < 1)
    return 0;
  while (*a == *b || (n = tolower (*a) - tolower (*b)) == 0)
    {
      if (*a == '\0' || --len < 1)
        return 0;
      a++, b++;
    }
  return n;
}



int create_local_directory(const char *dirname) {
	if ((mkdir(dirname, 0777)) != -1) {
		return SUCCESS;
	}
	else {
		printMsg(MESSAGETYPE_ERROR, "Problems creating directory \"%s\")\n", dirname);
		return FAILED;
	}
}



int string_startsWith(const char *string, const char *subString)
{
    size_t len = strlen(subString);

    if (my_strncasecmp(string, subString, len) == 0) {
            return (0);
    }
    return (-1);
}

int string_endsWith(const char *string, const char *subString)
{
    size_t lensub = strlen(subString);
    size_t len = strlen(string);
    if (len < lensub) {
        return -1;
    }

    if (my_strncasecmp(&string[len-lensub], subString, lensub) == 0) {
            return (0);
    }
    return (-1);
}


ReturnCode strip_dirname(const char *xmlFilename, char **dname, char **fname)
{
        char* dirBuffer = NULL;
        char* fileBuffer = NULL;
        char* tmpString = NULL;
        int   length    = 0;

#if _WIN32
/* use windows code to determine dirname and filename */

        /* variables for dirname/basename stripping */
        char drive[_MAX_DRIVE];
        char dir[_MAX_DIR];
        char flename[_MAX_FNAME];
        char ext[_MAX_EXT];

        _splitpath( xmlFilename, drive, dir, flename, ext );

        /* build full directory path */
        if(strlen(dir) == 0) {
            dirBuffer = (char *) malloc(sizeof(char) * strlen(drive) + strlen(dir) + 1);
            dirBuffer[0] = '\0';
        } else {
            dirBuffer = (char *) malloc(sizeof(char) * strlen(drive) + strlen(dir) + 3);
            dirBuffer[0] = '\0';
            strcat(dirBuffer, drive);
            strcat(dirBuffer, dir);
            strcat(dirBuffer, "/");
        }


        /* build full file name */
        fileBuffer = (char *) malloc(sizeof(char) * strlen(flename) + strlen(ext) + 3);
        fileBuffer[0] = '\0';
        strcat(fileBuffer, flename);
        strcat(fileBuffer, ext);

#else
/* use posix functions */

        /* copy string */
        length = strlen(xmlFilename);
        tmpString = (char*) malloc(sizeof(char)   * (length + 4));
        sprintf(tmpString, "%s", xmlFilename);

        /* build full file name */
        fileBuffer = (char *) malloc(sizeof(char) * (length + 4));
        sprintf(fileBuffer,"%s",basename(tmpString));
        free(tmpString);

        // allocate again beacuse basename modified tmpString
        tmpString = (char*) malloc(sizeof(char)   * (length + 4));
        sprintf(tmpString, "%s", xmlFilename);

        /* build full directory path */
        dirBuffer = (char *) malloc(sizeof(char)  * (length + 4));
        sprintf(dirBuffer,"%s/",dirname(tmpString));

        free(tmpString);

#endif
        /* copy strings from buffer */
        *fname = (char *) malloc((strlen(fileBuffer) + 1) * sizeof(char));
        *dname = (char *) malloc((strlen(dirBuffer) + 1) * sizeof(char));
        strcpy(*dname, dirBuffer);
        strcpy(*fname, fileBuffer);
        free(dirBuffer);
        free(fileBuffer);

        return SUCCESS;
}



int isPathRelative(const char *dirname)
{
    int returnCode = -1;

    if (dirname == NULL) {
        returnCode = -1;
        return returnCode;
    }

    if (string_startsWith(dirname, "file:") == 0) {
        if (string_startsWith(dirname, "file:///") == 0) {
            returnCode = -1;
        }
        else if (string_startsWith(dirname, "file://") == 0) {
            returnCode = 0;
        }
    }

    return returnCode;
}

int isLocalPathRelative(const char* dirname)
{
    if (isURIPath(dirname)==0) {
        return 1;
    }
#ifdef _WIN32
    if (PathIsRelative(dirname) == 1) {
        return 0;
    }
    else {
        return 1;
    }
#else
    if (string_startsWith(dirname, "/") == 0) {
        return 1;
    }
    else {
        return 0;
    }
#endif
}

int isURIPath(const char* path)
{
    // qualifier to detect if url or not
    const char * types[] = {
        "file://",
        "http://",
        "https://",
        "ftp://",
        "ssh://"
    };
    int ntypes = sizeof (types) / sizeof (const char *);
    int i;

    for (i = 0; i < ntypes; ++i) {
        if (string_startsWith(path, types[i]) == 0) {
            return 0;
        }
    }

    return 1;
}

char* uriToLocalPath(const char* URI)
{
    //we can only resolve the URI, if it starts with file://
    if (string_startsWith(URI, "file://") != 0) {
        return 0;
    }

    if (isPathRelative(URI) == 0) {
        // relative path, strip file://
        char* localPath = string_stripLeft(URI, 7);
        return localPath;
    }
    else {
        // absolute path,
        // on windows file:///c:/data/file.txt -> c:/data/file.txt
        // on linux   file:///data/file.txt -> /data/file.txt
#if _WIN32
        char* localPath = string_stripLeft(URI, 8);
        return localPath;
#else
        char* localPath = string_stripLeft(URI, 7);
        return localPath;
#endif
    }

}

char* loadFileToString(const char* path)
{
    char * buffer = 0;
    long length;
    FILE * f = fopen (path, "rb");
    if (f)
    {
      fseek (f, 0, SEEK_END);
      length = ftell (f);
      fseek (f, 0, SEEK_SET);
      buffer = malloc (length + sizeof(char)*3);
      if (buffer)
      {
        fread (buffer, 1, length, f);
      }
      fclose (f);

      // null terminate the string
      buffer[length] = '\0';
      return buffer;
    }
    else {
        return NULL;
    }
}

char* stringToLower(char* string)
{
    int i;
    int length;

    length = (int) strlen(string);

    for (i = 0; i < length; i++) {
        string[i] = (char) tolower(string[i]);
    }
    return string;
}

char* string_stripLeft(const char* string, int len)
{
    char *resultString = NULL;
    size_t x = 0;
    size_t stringLength = 0;
    int y = 0;

    stringLength = strlen(string);
    if (stringLength < (size_t) len) {
        printMsg(MESSAGETYPE_ERROR, "Error: could not strip %d bytes from string \"%s\"", len, string);
        return NULL;
    }

    resultString = (char *) malloc(sizeof(char) * (strlen(string) - len + 3));

    for(x = len; x < stringLength; x++) {
            resultString[y] = string[x];
            y++;
    }
    resultString[y]='\0';
    return resultString;
}
