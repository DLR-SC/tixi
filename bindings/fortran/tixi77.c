/*
* Copyright (C) 2015 German Aerospace Center (DLR/SC)
*
* Created: 2010-08-13 Markus Litz <Markus.Litz@dlr.de>
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
   @brief Implementation of wrappers of tixi public interface functions to be called from Fortran77/90 code.
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "tixi77.h"


/**
   @brief transformes a fortran string into a c string.
   @param fortranString a pointer to the memory occupied by the fortran string
   @param size of the fortran character variable holding the string
 */
char *makeCString(const char *fortranString, const int fortranStringLength)
{

  int iEnd;
  int cStringLength = -1;
  char *cString = NULL;

  if (fortranString != NULL) {

    /** Skip trailing blanks **/
    for (iEnd = fortranStringLength - 1; iEnd >= 0; iEnd--) {
      if (fortranString[iEnd] != ' ') {
        break;
      }
    }

    cStringLength = iEnd + 1;

    cString = (char *) malloc(sizeof(char) * (cStringLength + 1));

    if (!cString) {
      fprintf(stderr, "Fatal error: Failed to allocate memory in makeCstring");
      abort();
    }

    memcpy(cString, fortranString, cStringLength * sizeof(char));

    /** NULL terminate the c string **/

    cString[cStringLength] = '\0';
  }
  return cString;
}

void copyToFortranString(const char *cString, const int fortranStringLength, char *fortranString)
{
  if (cString) {

    size_t cStringLength = strlen(cString);

    if (cStringLength != 0) {

      size_t length = cStringLength < (size_t) fortranStringLength  /* prune string if too long */
          ? cStringLength
          : (size_t) fortranStringLength;

      size_t i;

      memcpy(fortranString, cString, length * sizeof(char));

      for (i = cStringLength; i < (size_t) fortranStringLength; i++) {
        fortranString[i] = ' ';
      }
    }
  }
}



void tixiGetVersion_f(char *version, int stringLength)
{
  char *cVersion = NULL;

  cVersion = tixiGetVersion();

  copyToFortranString(cVersion, stringLength, version);
}


void tixiOpenDocument_f(const char *xmlFilename, int *handle, ReturnCode *error, int lengthString1)
{
  char *cString;

  cString = makeCString(xmlFilename, lengthString1);

  *error = tixiOpenDocument(cString, handle);

  free(cString);
}

void tixiOpenDocumentRecursive_f(const char *xmlFilename, int *handle, ReturnCode *error, OpenMode *oMode, const int lengthString1)
{

  char *cString;

  cString = makeCString(xmlFilename, lengthString1);

  *error = tixiOpenDocumentRecursive(cString, handle, (OpenMode) *oMode);

  free(cString);
}

void tixiOpenDocumentFromHTTP_f(const char *xmlUrl, int *handle, ReturnCode *error, int lengthString1)
{
  char *cString;

  cString = makeCString(xmlUrl, lengthString1);

  *error = tixiOpenDocumentFromHTTP(cString, handle);

  free(cString);
}

void tixiCloseDocument_f(TixiDocumentHandle *handle, ReturnCode *error)
{
  *error = tixiCloseDocument(*handle);
}

void tixiCleanup_f(ReturnCode *error)
{
  *error = tixiCleanup();
}



void tixiCreateDocument_f(char *rootElementName, TixiDocumentHandle *handle, ReturnCode *error,
                          int lengthString1)
{
  char *cString;

  cString = makeCString(rootElementName, lengthString1);

  *error = tixiCreateDocument(cString, handle);

  free(cString);

}

void tixiSaveDocument_f(TixiDocumentHandle *handle,
                        char *xmlFilename, ReturnCode *error, int lengthString1)
{
  char *cString;

  cString = makeCString(xmlFilename, lengthString1);

  *error = tixiSaveDocument(*handle, cString);

  free(cString);
}

void tixiSaveCompleteDocument_f(TixiDocumentHandle *handle,
                                char *xmlFilename, ReturnCode *error, int lengthString1)
{
  char *cString;

  cString = makeCString(xmlFilename, lengthString1);

  *error = tixiSaveCompleteDocument(*handle, cString);

  free(cString);
}

void tixiSaveAndRemoveDocument_f(TixiDocumentHandle *handle,
                                char *xmlFilename, ReturnCode *error, int lengthString1)
{
  char *cString;

  cString = makeCString(xmlFilename, lengthString1);

  *error = tixiSaveAndRemoveDocument(*handle, cString);

  free(cString);
}

void tixiExportDocumentAsString_f(TixiDocumentHandle* handle,
                                  char* text, ReturnCode* error, int lengthString1)
{
    char *cText = NULL;

    *error = tixiExportDocumentAsString(*handle, &cText);

    if (*error == SUCCESS) {
        copyToFortranString(cText, lengthString1, text);
        if ((size_t) lengthString1 < strlen(cText) + 1) {
            *error = STRING_TRUNCATED;
        }
    }
}

void tixiImportFromString_f(const char* xmlImportString,
                             int* handle,
                             ReturnCode* error,
                             int lengthString1 )
{
  char *cString;

  cString = makeCString(xmlImportString, lengthString1);

  *error = tixiImportFromString(cString, handle);

  free(cString);
}

void tixiSchemaValidateFromFile_f(const TixiDocumentHandle *handle,
                          char *xsdFilename, ReturnCode *error, int lengthString1)
{
  char *cString;

  cString = makeCString(xsdFilename, lengthString1);
  *error = tixiSchemaValidateFromFile(*handle, cString);

  free(cString);
}

void tixiSchemaValidateFromString_f(const TixiDocumentHandle *handle,
                          char *xsdString, ReturnCode *error, int lengthString1)
{
  char *cString;

  cString = makeCString(xsdString, lengthString1);
  *error = tixiSchemaValidateFromString(*handle, cString);

  free(cString);
}

void tixiDTDValidate_f(const TixiDocumentHandle *handle,
                       char *DTDFilename, ReturnCode *error, const int lengthString1)
{
  assert(0);
}

void tixiGetTextElement_f(const TixiDocumentHandle *handle, char *elementPath,
                          char *text, ReturnCode *error, int lengthString1, int lengthString2)
{
  char *cElementPath;
  char *cText = NULL;

  cElementPath = makeCString(elementPath, lengthString1);

  *error = tixiGetTextElement(*handle, cElementPath, &cText);

  if (*error == SUCCESS) {
    copyToFortranString(cText, lengthString2, text);
    if ((size_t) lengthString2 < strlen(cText) + 1) {
      *error = STRING_TRUNCATED;
    }
  }

  free(cElementPath);
}

void tixiGetBooleanElement_f(const TixiDocumentHandle *handle,
                             char *elementPath,
                             int *boolean, ReturnCode *error, int elementPathLength)
{
  char *cElementPath;

  cElementPath = makeCString(elementPath, elementPathLength);

  *error = tixiGetBooleanElement(*handle, cElementPath, boolean);

  free(cElementPath);
}

void tixiGetIntegerElement_f(const TixiDocumentHandle *handle,
                             char *elementPath,
                             int *number, ReturnCode *error, int elementPathLength)
{
  char *cElementPath;

  cElementPath = makeCString(elementPath, elementPathLength);

  *error = tixiGetIntegerElement(*handle, cElementPath, number);

  free(cElementPath);
}

void tixiGetDoubleElement_f(const TixiDocumentHandle *handle, char *elementPath, double *number,
                            ReturnCode *error, int elementPathLength)
{
  char *cElementPath;

  cElementPath = makeCString(elementPath, elementPathLength);

  *error = tixiGetDoubleElement(*handle, cElementPath, number);

  free(cElementPath);
}

void tixiAddTextElement_f(const TixiDocumentHandle *handle,
                          char *parentPath,
                          char *elementName,
                          char *text,
                          ReturnCode *error,
                          int parentPathLength, int elementNameLength, int textLength)
{
  char *cParentPath = NULL;
  char *cElementName = NULL;
  char *cText = NULL;

  cParentPath = makeCString(parentPath, parentPathLength);
  cElementName = makeCString(elementName, elementNameLength);

  if (textLength != 0) {
    cText = makeCString(text, textLength);
  }

  *error = tixiAddTextElement(*handle, cParentPath, cElementName, cText);

  free(cParentPath);
  free(cElementName);
  if (cText) {
    free(cText);
  }
}

void tixiAddTextElementAtIndex_f(const TixiDocumentHandle *handle,
								  char *parentPath,
								  char *elementName,
								  char *text,
								  int index,
								  ReturnCode *error,
								  int parentPathLength, int elementNameLength, int textLength)
{
  char *cParentPath = NULL;
  char *cElementName = NULL;
  char *cText = NULL;

  cParentPath = makeCString(parentPath, parentPathLength);
  cElementName = makeCString(elementName, elementNameLength);

  if (textLength != 0) {
    cText = makeCString(text, textLength);
  }

  *error = tixiAddTextElementAtIndex(*handle, cParentPath, cElementName, cText, index);

  free(cParentPath);
  free(cElementName);
  if (cText) {
    free(cText);
  }
}

void tixiAddDoubleElement_f(const TixiDocumentHandle *handle,
                            char *parentPath,
                            char *elementName,
                            double *number,
                            char *format,
                            ReturnCode *error,
                            int parentPathLength, int elementNameLength, int formatLength)
{
  char *cParentPath = NULL;
  char *cElementName = NULL;
  char *cFormat = NULL;

  cParentPath = makeCString(parentPath, parentPathLength);
  cElementName = makeCString(elementName, elementNameLength);
  if (formatLength > 0) {
    cFormat = makeCString(format, formatLength);
  }
  *error = tixiAddDoubleElement(*handle, cParentPath, cElementName, *number, cFormat);

  if (cFormat) {
    free(cFormat);
  }
  free(cParentPath);
  free(cElementName);
}

void tixiAddIntegerElement_f(const TixiDocumentHandle *handle,
                             char *parentPath,
                             char *elementName,
                             int *number,
                             char *format,
                             ReturnCode *error,
                             int parentPathLength, int elementNameLength, int formatLength)
{
  char *cParentPath = NULL;
  char *cElementName = NULL;
  char *cFormat = NULL;


  cParentPath = makeCString(parentPath, parentPathLength);
  cElementName = makeCString(elementName, elementNameLength);
  if (formatLength > 0) {
    cFormat = makeCString(format, formatLength);
  }

  *error = tixiAddIntegerElement(*handle, cParentPath, cElementName, *number, cFormat);

  free(cParentPath);
  free(cElementName);
  if (cFormat) {
    free(cFormat);
  }
}


void tixiAddBooleanElement_f(const TixiDocumentHandle *handle,
                             char *parentPath,
                             char *elementName,
                             int *boolean,
                             char *format,
                             ReturnCode *error,
                             int parentPathLength, int elementNameLength, int formatLength)
{
  char *cParentPath = NULL;
  char *cElementName = NULL;
  char *cFormat = NULL;


  cParentPath = makeCString(parentPath, parentPathLength);
  cElementName = makeCString(elementName, elementNameLength);
  if (formatLength > 0) {
    cFormat = makeCString(format, formatLength);
  }

  *error = tixiAddIntegerElement(*handle, cParentPath, cElementName, *boolean, cFormat);

  free(cParentPath);
  free(cElementName);
  if (cFormat) {
    free(cFormat);
  }
}


void tixiUpdateTextElement_f(const TixiDocumentHandle *handle,
							  char *parentPath,
							  char *text,
							  ReturnCode *error,
							  int parentPathLength, int textLength)
{
  char *cParentPath = NULL;
  char *cText = NULL;

  cParentPath = makeCString(parentPath, parentPathLength);

  if (textLength != 0) {
    cText = makeCString(text, textLength);
  }

  *error = tixiUpdateTextElement(*handle, cParentPath, cText);

  free(cParentPath);
  if (cText) {
    free(cText);
  }
}

void tixiUpdateDoubleElement_f(const TixiDocumentHandle *handle,
							  char *parentPath,
							  double number,
							  char *format,
							  ReturnCode *error,
							  int parentPathLength, int formatLength, int textLength)
{
  char *cParentPath = NULL;
  char *cFormat = NULL;
  char *cText = NULL;

  cParentPath = makeCString(parentPath, parentPathLength);
  cFormat = makeCString(format, formatLength);

  *error = tixiUpdateDoubleElement(*handle, cParentPath, number, cFormat);

  free(cParentPath);
  free(cFormat);
  if (cText) {
    free(cText);
  }
}

void tixiUpdateIntegerElement_f(const TixiDocumentHandle *handle,
                              char *parentPath,
                              int number,
                              char *format,
                              ReturnCode *error,
                              int parentPathLength, int formatLength, int textLength)
{
  char *cParentPath = NULL;
  char *cFormat = NULL;
  char *cText = NULL;

  cParentPath = makeCString(parentPath, parentPathLength);
  cFormat = makeCString(format, formatLength);

  *error = tixiUpdateIntegerElement(*handle, cParentPath, number, cFormat);

  free(cParentPath);
  free(cFormat);
  if (cText) {
    free(cText);
  }
}

void tixiUpdateBooleanElement_f(const TixiDocumentHandle *handle,
                                  char *parentPath,
                                  int number,
                                  ReturnCode *error,
                                  int parentPathLength)
{
  char *cParentPath = NULL;
  char *cFormat = NULL;
  char *cText = NULL;

  cParentPath = makeCString(parentPath, parentPathLength);

  *error = tixiUpdateBooleanElement(*handle, cParentPath, number);

  free(cParentPath);
  free(cFormat);
  if (cText) {
    free(cText);
  }
}

void tixiRemoveElement_f(const TixiDocumentHandle *handle,
                         char *elementPath, ReturnCode *error, int lengthString1)
{
  char *CElementPath = NULL;

  CElementPath = makeCString(elementPath, lengthString1);

  *error  = tixiRemoveElement(*handle, CElementPath);

  free(CElementPath);
}


void tixiGetTextAttribute_f(const TixiDocumentHandle *handle,
                            char *elementPath,
                            char *attributeName,
                            char *text,
                            ReturnCode *error,
                            int elementPathLength, int attributeNameLength, int textLength)
{
  char *cElementPath = NULL;
  char *cAttributeName = NULL;
  char *cText = NULL;

  cElementPath = makeCString(elementPath, elementPathLength);
  cAttributeName = makeCString(attributeName, attributeNameLength);

  *error = tixiGetTextAttribute(*handle, cElementPath, cAttributeName, &cText);

  if (*error == SUCCESS) {
    copyToFortranString(cText, textLength, text);
    if ((size_t) textLength < strlen(cText) + 1) {
      *error = STRING_TRUNCATED;
    }
  }

  free(cElementPath);
  free(cAttributeName);
}

void tixiGetIntegerAttribute_f(const TixiDocumentHandle *handle,
                               char *elementPath,
                               char *attributeName,
                               int *number,
                               ReturnCode *error, int elementPathLength, int attributeNameLength)
{
  char *cElementPath = NULL;
  char *cAttributeName = NULL;

  cElementPath = makeCString(elementPath, elementPathLength);
  cAttributeName = makeCString(attributeName, attributeNameLength);

  *error = tixiGetIntegerAttribute(*handle, cElementPath, cAttributeName, number);

  free(cElementPath);
  free(cAttributeName);
}

void tixiGetBooleanAttribute_f(const TixiDocumentHandle *handle,
                               char *elementPath,
                               char *attributeName,
                               int *boolean,
                               ReturnCode *error, int elementPathLength, int attributeNameLength)
{
  char *cElementPath = NULL;
  char *cAttributeName = NULL;

  cElementPath = makeCString(elementPath, elementPathLength);
  cAttributeName = makeCString(attributeName, attributeNameLength);

  *error = tixiGetBooleanAttribute(*handle, cElementPath, cAttributeName, boolean);

  free(cElementPath);
  free(cAttributeName);
}

void tixiGetDoubleAttribute_f(const TixiDocumentHandle *handle,
                              char *elementPath,
                              char *attributeName,
                              double *number,
                              ReturnCode *error, int elementPathLength, int attributeNameLength)
{
  char *cElementPath = NULL;
  char *cAttributeName = NULL;

  *error = tixiGetDoubleAttribute(*handle, cElementPath, cAttributeName, number);

  free(cElementPath);
  free(cAttributeName);
}

void tixiAddTextAttribute_f(const TixiDocumentHandle *handle,
                            char *elementPath,
                            char *attributeName,
                            char *text,
                            ReturnCode *error,
                            int elementPathLength, int attributeNameLength, int textLength)
{
  char *cElementPath = NULL;
  char *cAttributeName = NULL;
  char *cText = NULL;

  cElementPath = makeCString(elementPath, elementPathLength);
  cAttributeName = makeCString(attributeName, attributeNameLength);
  cText = makeCString(text, textLength);

  *error = tixiAddTextAttribute(*handle, cElementPath, cAttributeName, cText);

  free(cElementPath);
  free(cAttributeName);
  free(cText);
}


void tixiAddDoubleAttribute_f(const TixiDocumentHandle *handle,
                              char *elementPath,
                              char *attributeName,
                              double *number,
                              char *format,
                              ReturnCode *error,
                              int elementPathLength, int attributeNameLength, int formatLength)
{

  char *cElementPath = NULL;
  char *cAttributeName = NULL;
  char *cFormat = NULL;

  cElementPath = makeCString(elementPath, elementPathLength);
  cAttributeName = makeCString(attributeName, attributeNameLength);

  if (formatLength > 0) {
    cFormat = makeCString(format, formatLength);
  }

  *error = tixiAddDoubleAttribute(*handle, cElementPath, cAttributeName, *number, cFormat);

  free(cElementPath);
  free(cAttributeName);
  if (cFormat) {
    free(cFormat);
  }
}

void tixiAddIntegerAttribute_f(const TixiDocumentHandle *handle,
                               char *elementPath,
                               char *attributeName,
                               int *number,
                               char *format,
                               ReturnCode *error,
                               int elementPathLength, int attributeNameLength, int formatLength)
{
  char *cElementPath = NULL;
  char *cAttributeName = NULL;
  char *cFormat = NULL;

  cElementPath = makeCString(elementPath, elementPathLength);
  cAttributeName = makeCString(attributeName, attributeNameLength);

  if (formatLength > 0) {
    cFormat = makeCString(format, formatLength);
  }

  *error = tixiAddDoubleAttribute(*handle, cElementPath, cAttributeName, *number, cFormat);

  free(cElementPath);
  free(cAttributeName);
  if (cFormat) {
    free(cFormat);
  }
}

void tixiRemoveAttribute_f(const TixiDocumentHandle *handle,
                           char *elementPath,
                           char *attributeName,
                           ReturnCode *error, int lengthString1, int lengthString2)
{
  char *CElementPath = NULL;
  char *CAttributeName = NULL;

  CElementPath = makeCString(elementPath, lengthString1);
  CAttributeName = makeCString(attributeName, lengthString2);

  *error  = tixiRemoveAttribute(*handle, CElementPath, CAttributeName);

  free(CElementPath);
  free(CAttributeName);
}


void tixiAddHeader_f(const TixiDocumentHandle *handle,
                     char *toolName,
                     char *version,
                     char *authorName,
                     ReturnCode *error, int toolNameLength, int versionLength, int authorNameLength)
{
  char *cToolName = NULL;
  char *cVersion = NULL;
  char *cAuthorName = NULL;

  cToolName = makeCString(toolName, toolNameLength);
  cVersion = makeCString(version, versionLength);
  cAuthorName = makeCString(authorName, authorNameLength);

  *error = tixiAddHeader(*handle, cToolName, cVersion, cAuthorName);

  free(cToolName);
  free(cVersion);
  free(cAuthorName);
}

void tixiGetNamedChildrenCount_f(const TixiDocumentHandle *handle,
                                 char *elementPath,
                                 char *childName,
                                 int *count,
                                 ReturnCode *error, int elementPathLength, int childNameLength)
{
  char *cElementPath = NULL;
  char *cChildName = NULL;

  cElementPath = makeCString(elementPath, elementPathLength);
  cChildName = makeCString(childName, childNameLength);

  *error = tixiGetNamedChildrenCount(*handle, cElementPath, cChildName, count);

  free(cElementPath);
  free(cChildName);
}

void tixiAddDoubleListWithAttributes_f(const TixiDocumentHandle *handle,
                                       char *parentPath,
                                       char *listName,
                                       char *childName,
                                       char *childAttributeName,
                                       double *values,
                                       char *format,
                                       char *attributes,
                                       int *nValues,
                                       ReturnCode *error,
                                       int parentPathLength,
                                       int listNameLength,
                                       int childNameLength,
                                       int childAttributeNameLength,
                                       int formatLength, int attributesLength)
{
  char *cParentPath = NULL;
  char *cListName = NULL;
  char *cChildName = NULL;
  char *cChildAttributeName = NULL;
  char *cFormat = NULL;
  char *cAttributes = NULL;

  size_t attributeStringLength;

  char *attributesString = NULL;
  char **attributeList = NULL;
  int i;

  cParentPath = makeCString(parentPath, parentPathLength);
  cListName = makeCString(listName, listNameLength);
  cChildName = makeCString(childName, childNameLength);
  cChildAttributeName = makeCString(childAttributeName, childAttributeNameLength);

  if (formatLength > 0) {
    cFormat = makeCString(format, formatLength);
  }

  cAttributes = makeCString(attributes, attributesLength);
  attributeStringLength = strlen(cAttributes) / (*nValues);
  attributesString = (char *) malloc(sizeof(char *) * (*nValues) * (attributeStringLength + 1));
  attributeList = (char **) malloc(sizeof(char *) * (*nValues));

  /* insert a \0 after each attribute and collect pointers to the beginning of each attribute */

  for (i = 0; i < *nValues; i++) {
    strncpy(&(attributesString[i * (attributeStringLength + 1)]),
            &(cAttributes[i * (attributeStringLength)]), attributeStringLength);
    attributesString[(i + 1) * (attributeStringLength + 1) - 1] = '\0';
    attributeList[i] = &(attributesString[i * (attributeStringLength + 1)]);
  }

  *error = tixiAddDoubleListWithAttributes(*handle, cParentPath, cListName, cChildName,
                                           cChildAttributeName, values, cFormat,
                                           (const char **) attributeList, *nValues);

  free(cParentPath);
  free(cListName);
  free(cChildName);
  free(cChildAttributeName);
  free(attributesString);
  free(cAttributes);
  free(attributeList);
  if (cFormat) {
    free(cFormat);
  }
}


void tixiAddPoint_f(const TixiDocumentHandle *handle,
                    char *parentPath,
                    double *x,
                    double *y,
                    double *z, char *format, int *error, int parentPathLength, int formatLength)
{
  char *cParentPath = NULL;
  char *cFormat = NULL;

  cParentPath = makeCString(parentPath, parentPathLength);

  if (formatLength > 0) {
    cFormat = makeCString(format, formatLength);
  }
  *error = tixiAddPoint(*handle, cParentPath, *x, *y, *z, cFormat);

  if (cFormat) {
    free(cFormat);
  }
  free(cParentPath);

}

void tixiGetPoint_f(const TixiDocumentHandle *handle,
                    char *pointParentPath,
                    int *index, double *x, double *y, double *z, int *error, int parentPathLength)
{
  char *cPointParentPath;

  cPointParentPath = makeCString(pointParentPath, parentPathLength);

  *error = tixiGetPoint(*handle, cPointParentPath, x, y, z);

  free(cPointParentPath);
}


void tixiCheckDocumentHandle_f(const TixiDocumentHandle *handle, ReturnCode *error)
{

  *error = tixiCheckDocumentHandle(*handle);

}

void tixiCheckElement_f(const TixiDocumentHandle *handle,
                        char *elementPath, ReturnCode *error, int elementPathLength)
{

  char *cElementPath = NULL;

  cElementPath = makeCString(elementPath, elementPathLength);

  *error = tixiCheckElement(*handle, cElementPath);

  free(cElementPath);

}


void tixiAddFloatVector_f(const TixiDocumentHandle *handle,
                          char *parentPath,
                          char *elementName,
                          double *vector,
                          int *numElements,
                          char* format,
                          int *error,
                          int parentPathLength,
                          int elementNameLength,
                          int formatLength)
{
    char *cParentPath = NULL;
    char *cElementName = NULL;
    char *cFormat = NULL;

    cParentPath = makeCString(parentPath, parentPathLength);
    cElementName = makeCString(elementName, elementNameLength);

    if (formatLength > 0) {
      cFormat = makeCString(format, formatLength);
    }

    *error = tixiAddFloatVector(*handle, cParentPath, cElementName, vector, *numElements, cFormat);

    free(cParentPath);
    free(cElementName);
    free(cFormat);
}


void tixiGetVectorSize_f(const TixiDocumentHandle *handle,
                         const char *vectorPath,
                         int *nElements,
                         int *error,
                         const int vectorPathLength)
{
    char *cVectorPath = NULL;
    cVectorPath = makeCString(vectorPath, vectorPathLength);

    *error = tixiGetVectorSize(*handle, cVectorPath, nElements);

    free(cVectorPath);
}


void tixiGetFloatVector_f(const TixiDocumentHandle *handle,
                          const char *vectorPath,
                          double *vectorArray,
                          const int *eNumber,
                          int *error,
                          const int vectorPathLength)
{
    char *cVectorPath = NULL;
    double *points = NULL;
    int i;

    cVectorPath = makeCString(vectorPath, vectorPathLength);
    *error = tixiGetFloatVector(*handle, cVectorPath, &points, *eNumber);
    for (i = 0; i < *eNumber; i ++) {
        vectorArray[i] = points[i]; /* copy from allocated to fortran pre-allocated mem */
    }
    /* don't free points, it's in the TIXI mem list */
    free(cVectorPath);
}


void tixiGetArrayDimensions_f(const TixiDocumentHandle *handle,
                              const char *arrayPath,
                              int *dimensions,  /* out */
                              int *error,   /* out */
                              const int arrayPathLength)
{
    char *cArrayPath = makeCString(arrayPath, arrayPathLength);
    *error = tixiGetArrayDimensions(*handle, cArrayPath, dimensions);
    free(cArrayPath);
}


void tixiGetArrayDimensionNames_f(const TixiDocumentHandle *handle,
                                  const char *arrayPath,
                                  char *dimensionNames,    /* out */
                                  const int *strings,
                                  const int *stringSize,
                                  int *error,   /* out */
                                  const int arrayPathLength)
{
    char *cArrayPath = makeCString(arrayPath, arrayPathLength);
    char **tmpNames = (char**) malloc(*strings * sizeof(char*));
    int i = 0;
    int j = 0;
    int k = 0;
    char empty = ' ';

    *error = tixiGetArrayDimensionNames(*handle, cArrayPath, tmpNames); /* allocates each string */

    if (*error == SUCCESS) {
        for (i = 0; i < *strings; i ++) {
            for (j = 0; j < *stringSize; j ++) {
                memcpy(&dimensionNames[i + j * *stringSize], &tmpNames[i][j], 1);
                free(tmpNames[i]);  /* no longer needed */
                if (tmpNames[i][j] == '\0') {
                    for (k = j + 1; k < *stringSize; k ++) {
                        memcpy(&dimensionNames[i + j * *stringSize], &empty, 1);
                    }
                    break;
                }
            }
        }
    }
    free(tmpNames);
    free(cArrayPath);
}


void tixiGetArrayDimensionSizes_f(const TixiDocumentHandle *handle,
                                  const char *arrayPath,
                                  int *sizes,   /* out */
                                  int *arraySize,   /* out */
                                  int *error,   /* out */
                                  const int arrayPathLength)
{
    char *cArrayPath = makeCString(arrayPath, arrayPathLength);
    *error = tixiGetArrayDimensionSizes(*handle, cArrayPath, sizes, arraySize);
    free(cArrayPath);
}


void tixiGetArrayDimensionValues_f(const TixiDocumentHandle *handle,
                                   const char *arrayPath,
                                   const int *dimension,
                                   double *dimensionValues, /* out */
                                   int *error,  /* out */
                                   const int arrayPathLength)
{
    char *cArrayPath = makeCString(arrayPath, arrayPathLength);
    *error = tixiGetArrayDimensionValues(*handle, cArrayPath, *dimension, dimensionValues);
    free(cArrayPath);
}


void tixiGetArrayParameters_f(const TixiDocumentHandle *handle,
                              const char *arrayPath,
                              int *parameters,  /* out */
                              int *error,   /* out */
                              const int arrayPathLength)
{
    char *cArrayPath = makeCString(arrayPath, arrayPathLength);
    *error = tixiGetArrayParameters(*handle, cArrayPath, parameters);
    free(cArrayPath);
}


void tixiGetArrayParameterNames_f(const TixiDocumentHandle *handle,
                                  const char *arrayPath,
                                  char *parameterNames,    /* out */
                                  const int *strings,
                                  const int *stringSize,
                                  int *error,   /* out */
                                  const int arrayPathLength)
{
    char *cArrayPath = makeCString(arrayPath, arrayPathLength);
    char **tmpNames = (char**) malloc(*strings * sizeof(char*));
    int i = 0;
    int j = 0;
    int k = 0;
    char empty = ' ';

    *error = tixiGetArrayParameterNames(*handle, cArrayPath, tmpNames); /* allocates each string */

    if (*error == SUCCESS) {
        for (i = 0; i < *strings; i ++) {
            for (j = 0; j < *stringSize; j ++) {
                memcpy(&parameterNames[i + j * *stringSize], &tmpNames[i][j], 1);
                free(tmpNames[i]);  /* no longer needed */
                if (tmpNames[i][j] == '\0') {
                    for (k = j + 1; k < *stringSize; k ++) {
                        memcpy(&parameterNames[i + j * *stringSize], &empty, 1);
                    }
                    break;
                }
            }
        }
    }
    free(tmpNames);
    free(cArrayPath);
}


void tixiGetArray_f(const TixiDocumentHandle *handle,
                    const char *arrayPath,
                    const char *elementName,
                    const int numElements, /* in */
                    double *values, /* out */
                    int *error, /* out */
                    const int arrayPathLength,
                    const int elementNameLength)
{
    char *cArrayPath = makeCString(arrayPath, arrayPathLength);
    char *cElementName = makeCString(elementName, elementNameLength);
    int i = 0;

    double * tmpValues = NULL;
    *error = tixiGetArray(*handle, cArrayPath, cElementName, numElements, &tmpValues);

    /* copy to preallocated fortran array, don't free tmpValues as tixi does it automatically */
    for(i = 0; i < numElements; ++i)
        values[i] = tmpValues[i];

    free(cElementName);
    free(cArrayPath);
}


void tixiGetArrayValue_f(const double *array,
                         const int *dimSize,
                         const int *dimPos,
                         const int *dims,
                         double *value)
{
    *value = tixiGetArrayValue(array, dimSize, dimPos, *dims);
}


void tixiCreateElement_f(const TixiDocumentHandle *handle,
                         char *parentPath,
                         char *elementName,
                         int *error,
                         int parentPathLength,
                         int elementNameLength)
{
    char *cParentPath = NULL;
    char *cElementName = NULL;

    cParentPath = makeCString(parentPath, parentPathLength);
    cElementName = makeCString(elementName, elementNameLength);

    *error = tixiCreateElement(*handle, cParentPath, cElementName);

    free(cElementName);
    free(cParentPath);
}


void tixiCreateElementAtIndex_f(const TixiDocumentHandle *handle,
								 char *parentPath,
								 char *elementName,
								 int index,
								 int *error,
								 int parentPathLength,
								 int elementNameLength)
{
    char *cParentPath = NULL;
    char *cElementName = NULL;

    cParentPath = makeCString(parentPath, parentPathLength);
    cElementName = makeCString(elementName, elementNameLength);

    *error = tixiCreateElementAtIndex(*handle, cParentPath, cElementName, index);

    free(cElementName);
    free(cParentPath);
}

void tixiCheckAttribute_f(const TixiDocumentHandle *handle,
						  char *elementPath,
						  char *attributeName,
						  ReturnCode* error,
						  int elementPathLength,
						  int attributeNameLength)
{
    char *cElementPath = NULL;
    char *cAttributeName = NULL;

    cElementPath = makeCString(elementPath, elementPathLength);
    cAttributeName = makeCString(attributeName, attributeNameLength);

    *error = tixiCheckAttribute(*handle, cElementPath, cAttributeName);

    free(cElementPath);
    free(cAttributeName);
}


void tixiXSLTransformationToFile_f(TixiDocumentHandle* handle,
                                    char* stylesheetFilename,
                                    char* outputFilename,
                                    ReturnCode* error,
                                    int elementPathLength1,
                                    int elementPathLength2)
{
    char *cstylesheetFilename = NULL;
    char *coutputFilename = NULL;

    cstylesheetFilename = makeCString(stylesheetFilename, elementPathLength1);
    coutputFilename = makeCString(outputFilename, elementPathLength2);

    *error = tixiXSLTransformationToFile(*handle, cstylesheetFilename, coutputFilename);

    free(cstylesheetFilename);
    free(coutputFilename);
}


void tixiUsePrettyPrint_f(TixiDocumentHandle *handle,
                            int *usePrettyPrint,
                            int *error)
{
    *error = tixiUsePrettyPrint(*handle, *usePrettyPrint);
}


void tixiUIDCheckDuplicates_f(TixiDocumentHandle *handle,
								int *error)
{
    *error = tixiUIDCheckDuplicates(*handle);
}

void tixiUIDCheckLinks_f(TixiDocumentHandle *handle,
								int *error)
{
    *error = tixiUIDCheckLinks(*handle);
}

void tixiUIDGetXPath_f(TixiDocumentHandle *handle,
					   char* uID,
					   char* xPath,
					   ReturnCode* error,
					   int lengthString1,
					   int lengthString2)
{
	char *cElementPath;
	char *cText = NULL;

	cElementPath = makeCString(uID, lengthString1);

	*error = tixiUIDGetXPath(*handle, cElementPath, &cText);

	if (*error == SUCCESS) {
		copyToFortranString(cText, lengthString2, xPath);
		if ((size_t) lengthString2 < strlen(cText) + 1) {
			*error = STRING_TRUNCATED;
		}
	}

	free(cElementPath);
}


void tixiUIDCheckExists_f(TixiDocumentHandle *handle,
						   char* uID,
						   ReturnCode* error,
						   int lengthString1)
{
	char *cUID;

	cUID = makeCString(uID, lengthString1);
	*error = tixiUIDCheckExists(*handle, cUID);
}


void tixiUIDSetToXPath_f(const TixiDocumentHandle* handle,
								char* elementPath,
								char* uID,
								ReturnCode* error,
								int lengthString1,
								int lengthString2)
{
	  char *cElementPath = NULL;
	  char *cText = NULL;

	  cElementPath = makeCString(elementPath, lengthString1);
	  cText = makeCString(uID, lengthString2);

	  *error = tixiUIDSetToXPath(*handle, cElementPath, cText);

	  free(cElementPath);
}


void tixiXPathEvaluateNodeNumber_f(const TixiDocumentHandle* handle,
									char* xpathExpression,
									int* number,
									ReturnCode* error,
									int lengthString1)
{
	char *cElementPath;

	cElementPath = makeCString(xpathExpression, lengthString1);
	*error = tixiXPathEvaluateNodeNumber(*handle, cElementPath, number);
}


void tixiXPathExpressionGetTextByIndex_F(const TixiDocumentHandle* handle,
										char* xpathExpression,
										int* index,
										char* text,
										ReturnCode* error,
										int lengthString1,
										int lengthString2)
{
	char *cElementPath;
	char *cText = NULL;

	cElementPath = makeCString(xpathExpression, lengthString1);

	*error = tixiXPathExpressionGetTextByIndex(*handle, cElementPath, *index, &cText);

	if (*error == SUCCESS) {
		copyToFortranString(cText, lengthString2, text);
		if ((size_t) lengthString2 < strlen(cText) + 1) {
			*error = STRING_TRUNCATED;
		}
	}
	free(cElementPath);
}


void tixiGetChildNodeName_F(const TixiDocumentHandle* handle,
                                        char* parentPath,
                                        int* index,
                                        char* text,
                                        ReturnCode* error,
                                        int lengthString1,
                                        int lengthString2)
{
    char *cElementPath;
    char *cText = NULL;

    cElementPath = makeCString(parentPath, lengthString1);

    *error = tixiGetChildNodeName(*handle, cElementPath, *index, &cText);

    if (*error == SUCCESS) {
        copyToFortranString(cText, lengthString2, text);
        if ((size_t) lengthString2 < strlen(cText) + 1) {
            *error = STRING_TRUNCATED;
        }
    }
    free(cElementPath);
}

void tixiGetAttributeName_f(const TixiDocumentHandle* handle,
                            char* elementPath,
                            int* index,
                            char* text,
                            ReturnCode* error,
                            int lengthString1,
                            int lengthString2) {

    char *cElementPath;
    char *cText = NULL;

    cElementPath = makeCString(elementPath, lengthString1);

    *error = tixiGetAttributeName(*handle, cElementPath, *index, &cText);

    if (*error == SUCCESS) {
        copyToFortranString(cText, lengthString2, text);
        if ((size_t) lengthString2 < strlen(cText) + 1) {
            *error = STRING_TRUNCATED;
        }
    }
    free(cElementPath);
}

void tixiGetNumberOfChilds_f(const TixiDocumentHandle* handle,
                                        char* elementPath,
                                        int * nChilds,
                                        ReturnCode* error,
                                        int lengthString1){

    char *cElementPath = makeCString(elementPath, lengthString1);
    *error = tixiGetNumberOfChilds(*handle, cElementPath, nChilds);
    free(cElementPath);
}

void tixiGetNumberOfAttributes_f(const TixiDocumentHandle* handle,
                                        char* elementPath,
                                        int * nAttributes,
                                        ReturnCode* error,
                                        int lengthString1){

    char *cElementPath = makeCString(elementPath, lengthString1);
    *error = tixiGetNumberOfAttributes(*handle, cElementPath, nAttributes);
    free(cElementPath);
}

