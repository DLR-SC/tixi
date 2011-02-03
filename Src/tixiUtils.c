/* 
* Copyright (C) 2007-2011 German Aerospace Center (DLR/SC)
*
* Created: 2010-08-13 Markus Litz <Markus.Litz@dlr.de>
* Changed: $Id: tixiUtils.c 4392 2011-02-03 08:52:55Z litz_ma $ 
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
   @file Utility functions used to implement the interface.
*/
#include "tixiUtils.h"
#include "tixiInternal.h"


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
	if ((mkdir(dirname)) != -1) {
		return SUCCESS;
	}
	else {
		printf("Problems creating directory \"%s\")\n", dirname);
		return FAILED;
	}
}



int string_startsWith(char *string, char *subString)
{
    int len = strlen(subString);

    if (my_strncasecmp(string, subString, len) == 0) {
            return (0);
    }
    return (-1);
}


ReturnCode strip_dirname(char *xmlFilename, char **dname, char **fname)
{
        char* dirBuffer = NULL;
        char* fileBuffer = NULL;
        char* tmpString = NULL;

#if WIN32
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
        tmpString = (char*) malloc(sizeof(char) * strlen(xmlFilename) +1);
        strcpy(tmpString, xmlFilename);

        /* build full directory path */
        dirBuffer = (char *) malloc(sizeof(char) * strlen(dirname(tmpString)) + 2);
        strcpy(dirBuffer, dirname(tmpString));
        strcat(dirBuffer, "/");

        /* build full file name */
        fileBuffer = (char *) malloc(sizeof(char) * strlen(basename(tmpString)) + 1);
        strcpy(fileBuffer, basename(tmpString));

        free(tmpString);

#endif
        /* copy strings from buffer */
        *fname = (char *) malloc((strlen(fileBuffer) + 1) * sizeof(char));
        *dname = (char *) malloc((strlen(dirBuffer) + 1) * sizeof(char));
        strcpy(*dname, dirBuffer);
        strcpy(*fname, fileBuffer);

        return SUCCESS;
}



int isPathRelative(char *dirname)
{
    int returnCode = -1;

    if (dirname == NULL) {
        returnCode = -1;
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

char* string_stripLeft(char* string, int len)
{
    char *resultString = NULL;
    int x = 0;
    int stringLength = 0;
    int y = 0;

    stringLength = strlen(string);
    if (stringLength < len) {
        fprintf(stderr, "Error: could not strip %d bytes from string \"%s\"", len, string);
        return NULL;
    }

    resultString = (char *) malloc(sizeof(char) * strlen(string) - len);

    for(x = len; x < stringLength; x++) {
            resultString[y] = string[x];
            y++;
    }
    resultString[y]='\0';
    return resultString;
}
