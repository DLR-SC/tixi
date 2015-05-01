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
#define _CRT_SECURE_NO_WARNINGS
#include <assert.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>

#include "tixi.h"
#include "tixiData.h"
#include "tixiInternal.h"
#include "uidHelper.h"
#include "tixiUtils.h"
#include "webMethods.h"
#include "libxml/xmlschemas.h"

/**
   @file Auxiliary routines used to implement the interface.
*/


#ifndef max
#define max(a, b) (((a) > (b)) ? (a) : (b))
#endif


extern TixiPrintMsgFnc printMsg;

InternalReturnCode clearMemoryList(TixiDocument *document)
{

  TixiMemoryListEntry *current = document->memoryListHead;

  while (current) {

    TixiMemoryListEntry *next = (TixiMemoryListEntry *) current->next;
    free(current->memory);
    free(current);
    current = next;
  }
  document->memoryListHead = NULL;
  document->memoryListTail = NULL;
  return SUCCESS;
}

void freeTixiDocument(TixiDocument *document)
{
  if (document->xmlFilename) {
    free(document->xmlFilename);
    document->xmlFilename = NULL;
  }

  if (document->validationFilename) {
    free(document->validationFilename);
    document->validationFilename = NULL;
  }

   if (document->dirname) {
    free(document->dirname);
    document->dirname = NULL;
  }

  if (document->filename) {
    free(document->filename);
    document->filename = NULL;
  }
  clearMemoryList(document);
  uid_clearUIDList(document);

  xmlFreeDoc(document->docPtr);

  free(document);
}

InternalReturnCode addDocumentToList(TixiDocument *document, TixiDocumentHandle *handle)
{

  TixiDocumentListEntry *currentEntry = documentListHead;
  static int handleCounter = 0;

  if (documentListHead) {

    while (currentEntry->next) {
      currentEntry = (TixiDocumentListEntry *) currentEntry->next;
    }

    currentEntry->next = (struct TixiDocumentListEntry *)
      malloc(sizeof(TixiDocumentListEntry));

    if (!currentEntry->next) {

      return MEMORY_ALLOCATION_FAILED;
    }

    currentEntry = (TixiDocumentListEntry *) currentEntry->next;
  }
  else {

    currentEntry = (TixiDocumentListEntry *) malloc(sizeof(TixiDocumentListEntry));

    if (currentEntry) {
      documentListHead = currentEntry;
    }
    else {
      return MEMORY_ALLOCATION_FAILED;
    }
  }

  currentEntry->document = document;
  currentEntry->next = NULL;

  handleCounter++;
  *handle = handleCounter;

  return SUCCESS;
}

ReturnCode removeDocumentFromList(TixiDocumentHandle handle)
{

  TixiDocumentListEntry *currentEntry = documentListHead;
  TixiDocumentListEntry *previousEntry = NULL;

  ReturnCode returnValue = SUCCESS;

  if (documentListHead) {

    while (currentEntry) {

      if (currentEntry->document->handle != handle) {
        previousEntry = currentEntry;
        currentEntry = (TixiDocumentListEntry *) currentEntry->next;
      }
      else {
        if (previousEntry) {
          previousEntry->next = currentEntry->next;
          free(currentEntry);
          currentEntry = NULL;
          returnValue = SUCCESS;
        }
        else {
          documentListHead = (TixiDocumentListEntry *) currentEntry->next;
          free(currentEntry);
          currentEntry = NULL;
          returnValue = SUCCESS;
        }
      }
    }
  }
  else {
    returnValue = FAILED;
  }

  return returnValue;
}

TixiDocument *getDocument(TixiDocumentHandle handle)
{

  TixiDocumentListEntry *currentEntry = documentListHead;
  TixiDocument *returnValue = NULL;

  if (currentEntry) {
    while (currentEntry && returnValue == NULL) {

      if (currentEntry->document->handle != handle) {
        currentEntry = (TixiDocumentListEntry *) currentEntry->next;
      }
      else {
        returnValue = currentEntry->document;
      }
    }
  }
  else {
    returnValue = NULL;
  }

  return returnValue;
}

InternalReturnCode addToMemoryList(TixiDocument *document, void *memory)
{

  TixiMemoryListEntry *currentEntry = document->memoryListTail;

  if (currentEntry) {

    currentEntry->next = (struct TixiMemoryListEntry *) malloc(sizeof(TixiMemoryListEntry));

    if (currentEntry->next) {
      currentEntry = (TixiMemoryListEntry *) currentEntry->next;
      document->memoryListTail = currentEntry;
    }
    else {
      return MEMORY_ALLOCATION_FAILED;
    }
  }
  else {

    currentEntry = (TixiMemoryListEntry *) malloc(sizeof(TixiMemoryListEntry));

    if (currentEntry) {
      document->memoryListHead = currentEntry;
      document->memoryListTail = currentEntry;
    }
    else {
      return MEMORY_ALLOCATION_FAILED;
    }
  }

  currentEntry->memory = memory;
  currentEntry->next = NULL;

  return SUCCESS;
}

ReturnCode checkExistence(const xmlDocPtr xmlDocument, const char *elementPath, xmlXPathObjectPtr * xpathObject)
{

  xmlXPathContextPtr xpathContext = NULL;
  *xpathObject = NULL;

  /* Create xpath evaluation context */
  xpathContext = xmlXPathNewContext(xmlDocument);
  if (!xpathContext) {
    printMsg(MESSAGETYPE_ERROR, "Error: Unable to create new XPath context.\n");
    return FAILED;
  }

  *xpathObject = xmlXPathEvalExpression((const xmlChar *) elementPath, xpathContext);
  if (!(*xpathObject)) {
    printMsg(MESSAGETYPE_ERROR, "Error: Invalid XPath expression \"%s\"\n", elementPath);
    //xmlXPathFreeObject(*xpathObject);
    xmlXPathFreeContext(xpathContext);
    *xpathObject = NULL;
    return INVALID_XPATH;
  }

  if (xmlXPathNodeSetIsEmpty((*xpathObject)->nodesetval)) {
    xmlXPathFreeContext(xpathContext);
    xmlXPathFreeObject(*xpathObject);
    *xpathObject = NULL;
    return ELEMENT_NOT_FOUND;
  }

  xmlXPathFreeContext(xpathContext);
  return SUCCESS;
}



ReturnCode checkExternalNode(const xmlNodePtr element) {
    ReturnCode error = FAILED;
    xmlChar * xmlAttribute = NULL;

    /* check attribute externalDataNodePath */
    xmlAttribute = xmlGetProp(element, (const xmlChar *) EXTERNAL_DATA_XML_ATTR_NODEPATH);
    if (xmlAttribute != NULL) {
        error = SUCCESS;
    } else {
        return FAILED;
    }
    xmlFree(xmlAttribute);

    /* check attribute externalFileName */
    xmlAttribute = xmlGetProp(element, (const xmlChar *) EXTERNAL_DATA_XML_ATTR_FILENAME);
    if (xmlAttribute != NULL) {
        error = SUCCESS;
    } else {
        return FAILED;
    }
    xmlFree(xmlAttribute);

    /* check attribute externalDataDirectory */
    xmlAttribute = xmlGetProp(element, (const xmlChar *) EXTERNAL_DATA_XML_ATTR_DIRECTORY);
    if (xmlAttribute != NULL) {
        error = SUCCESS;
    } else {
        return FAILED;
    }
    xmlFree(xmlAttribute);
    return error;
}


ReturnCode checkElement(const xmlDocPtr xmlDocument, const char *elementPathDirty,
                        xmlNodePtr * element, xmlXPathObjectPtr * xpathObject)
{

  xmlXPathContextPtr xpathContext = NULL;
  xmlNodeSetPtr nodes = NULL;
  char elementPath[1024];
  
  *xpathObject = NULL;

  /* remove trailing slash */
  strncpy(elementPath, elementPathDirty, 1024);
  if(strlen(elementPath) > 1 && elementPath[strlen(elementPath)-1] == '/'){
      elementPath[strlen(elementPath)-1] = '\0';
  }
  
  /* Create xpath evaluation context */
  xpathContext = xmlXPathNewContext(xmlDocument);
  if (!xpathContext) {
    printMsg(MESSAGETYPE_ERROR, "Error: Unable to create new XPath context.\n");
    return FAILED;
  }

  /* Evaluate Expression */
  *xpathObject = xmlXPathEvalExpression((xmlChar *) elementPath, xpathContext);
  if (!(*xpathObject)) {
    printMsg(MESSAGETYPE_ERROR, "Error: Invalid XPath expression \"%s\"\n", elementPath);
    xmlXPathFreeContext(xpathContext);
    return INVALID_XPATH;
  }

  if (xmlXPathNodeSetIsEmpty((*xpathObject)->nodesetval)) {
    xmlXPathFreeContext(xpathContext);
    xmlXPathFreeObject(*xpathObject);
    *xpathObject = NULL;
    return ELEMENT_NOT_FOUND;
  }

  nodes = (*xpathObject)->nodesetval;
  assert(nodes);

  if (nodes->nodeNr > 1) {
    printMsg(MESSAGETYPE_ERROR,
            "Error: Element chosen by XPath \"%s\" expression is not unique. \n", elementPath);
    xmlXPathFreeContext(xpathContext);
    xmlXPathFreeObject(*xpathObject);
    *xpathObject = NULL;
    return ELEMENT_PATH_NOT_UNIQUE;
  }

  assert(nodes->nodeTab[0]);

  if (nodes->nodeTab[0]->type == XML_ELEMENT_NODE || nodes->nodeTab[0]->type == XML_DOCUMENT_NODE) {
    *element = nodes->nodeTab[0];
    xmlXPathFreeContext(xpathContext);
    return SUCCESS;
  }
  else {
    printMsg(MESSAGETYPE_ERROR,
            "Error: XPath expression \"%s\"does not point to an element node.\n", elementPath);
    xmlXPathFreeContext(xpathContext);
    xmlXPathFreeObject(*xpathObject);
    *xpathObject = NULL;
    return NOT_AN_ELEMENT;
  }
}


ReturnCode getCoordinateValue(xmlDocPtr xmlDocument, char *pointPath,
                              int pointIndex, char *name, int ignoreError, double *value)
{
  xmlNodePtr coordinate = NULL;
  ReturnCode error = -1;
  xmlXPathObjectPtr xpathObject = NULL;
  size_t suffixStringLength = 0;
  size_t pointPathLength = strlen(pointPath);
  size_t coordinatePathStringLength = 0;
  char *coordinatePath = NULL;
  char *suffixString = buildString("[%d]/%1s", pointIndex, name);

  if (!suffixString) {
    printMsg(MESSAGETYPE_ERROR, "Internal Error: Failed to allocate memory in getCoordinateValue.\n");
    exit(1);
  }

  suffixStringLength = strlen(suffixString);

  coordinatePathStringLength = suffixStringLength + pointPathLength + 1;

  coordinatePath = (char *) malloc(coordinatePathStringLength * sizeof(char));
  strcpy(coordinatePath, pointPath);
  strcat(coordinatePath, suffixString);

  error = checkElement(xmlDocument, coordinatePath, &coordinate, &xpathObject);

  free(coordinatePath);
  free(suffixString);
  coordinatePath = NULL;
  suffixString = NULL;
  if (!error) {

    char *textPtr = (char *) xmlNodeListGetString(xmlDocument, coordinate->children, 0);

    if (textPtr) {
      *value = atof(textPtr);
      xmlFree(textPtr);
    }
    else {
      error = FAILED;
    }
    xmlXPathFreeObject(xpathObject);
  }
  else {
    if (!ignoreError) {
      printMsg(MESSAGETYPE_ERROR,
              "Internal Error: point element \"%s\" has no %s-coordinate.\n", pointPath, name);
    }
    error = COORDINATE_NOT_FOUND;
  }

  return error;
}

ReturnCode getPoint(const TixiDocumentHandle handle, const char *parentPath, const int pointIndex,
                    int ignoreMissingElements, double *x, double *y, double *z)
{
  ReturnCode error = -1;
  ReturnCode errorX = -1;
  ReturnCode errorY = -1;
  ReturnCode errorZ = -1;

  TixiDocument *document = getDocument(handle);
  xmlDocPtr xmlDocument = NULL;
  xmlXPathObjectPtr xpathParentObject = NULL;
  xmlNodePtr parent = NULL;
  char *pointPath = NULL;
  char *coordinatePathBuffer = NULL;
  int nPointElements = 0;

  if (!document) {
    printMsg(MESSAGETYPE_ERROR, "Error: Invalid document handle.\n");
    return INVALID_HANDLE;
  }

  if (pointIndex < 1) {
    printMsg(MESSAGETYPE_ERROR, "Error: Invalid point index %d\n", pointIndex);
    return INDEX_OUT_OF_RANGE;
  }

  xmlDocument = document->docPtr;

  error = checkElement(xmlDocument, parentPath, &parent, &xpathParentObject);
  xmlXPathFreeObject(xpathParentObject);

  if (!error) {

    xmlXPathObjectPtr xpathPointObject = NULL;

    size_t lengthPointPath = (strlen(parentPath) +      /* length of path to parent   */
                              1);      				    /* trailing \0                */

    pointPath = (char *) malloc(sizeof(char) * lengthPointPath);

    strcpy(pointPath, parentPath);

    /* pad with trailing blanks, so we can check for the size in getCoordinateValue */

    error = checkExistence(xmlDocument, pointPath, &xpathPointObject);

    if (xpathPointObject) {
      nPointElements = xpathPointObject->nodesetval->nodeNr;
      xmlXPathFreeObject(xpathPointObject);
    }

    if (error == ELEMENT_NOT_FOUND) {
      printMsg(MESSAGETYPE_ERROR, "Error: No point element found in element \"%s\".\n", parentPath);
      error = NO_POINT_FOUND;
    }
    else if (!error) {
      if (pointIndex > nPointElements) {
        printMsg(MESSAGETYPE_ERROR, "Error: Index (%d) larger than number of point elements.\n", pointIndex);
        error = INDEX_OUT_OF_RANGE;
      }
      else {
		  errorX = getCoordinateValue(xmlDocument, pointPath, pointIndex, "x", ignoreMissingElements, x);
          errorY = getCoordinateValue(xmlDocument, pointPath, pointIndex, "y", ignoreMissingElements, y);
          errorZ = getCoordinateValue(xmlDocument, pointPath, pointIndex, "z", ignoreMissingElements, z);

          if( errorX && errorY && errorZ) {
                error = NO_POINT_FOUND;
		  } else {
			  error = SUCCESS;
		  }
      }

	}
  }

  free(coordinatePathBuffer);
  free(pointPath);
  return error;
}



void checkLibxml2Version()
{

  static int testPerformed = 0;

  if (!testPerformed) {
    LIBXML_TEST_VERSION;
  }

}


char *buildString(const char *format, ...)
{

  int bufferLength = 10;        /* initial guess for the buffer length */
  int nChars = -1;
  va_list variableList;
  char *buffer = NULL;

  buffer = (char *) malloc(bufferLength * sizeof(char));

  if (buffer) {

    while (nChars < 0) {

      /* try to print into buffer */

      va_start(variableList, format);
      nChars = VSNPRINTF(buffer, bufferLength, format, variableList);
      va_end(variableList);

      if (nChars < bufferLength && nChars > -1) {
        break;
      }

      /* msvc and glibc < 2.1: VSNPRINTF returns a negative integer on failure */
      if (nChars < 0) {

        bufferLength *= 2;
        //PRINT_DEBUG2("nChar = %d buffer length = %d, reallocating\n", nChars, bufferLength);
      }
      /*
         glibc 2.0: VSNPRINTF returns the length ( numbers of chars) of the result
         without the trailing \0 if the length would not exceed bufferLength.
      */
      else {

        bufferLength = nChars + 1;
        //PRINT_DEBUG2("nChar = %d buffer length = %d, reallocating\n", nChars, bufferLength);
      }

      nChars = -1;
      buffer = (char *) realloc(buffer, bufferLength * sizeof(char));

      if (!buffer) {
        break;
      }
    }
  }
  return buffer;
}



ReturnCode openExternalFiles(TixiDocument *aTixiDocument, int *number)
{
	ReturnCode error = 0;
    TixiDocumentHandle handle = aTixiDocument->handle;
    //xmlNodePtr cur_node = NULL;
    char *externalDataNodePath = NULL;
    char *externalDataDirectoryPath = NULL;
    char *externalDataDirectory = NULL;
    char *externalFileName = NULL;
    char *externalFullFileName = NULL;
    char *ptrFilename = NULL;
    char* newDocumentString = NULL;
    int externalFileCount = 0;
    xmlXPathContextPtr xpathContext = NULL;
    xmlXPathObjectPtr xpathObject = NULL;
    xmlNodeSetPtr nodes = NULL;
    int iNode, i;
    xmlDocPtr xmlDocument = NULL;
    xmlNodePtr nodeToInsert = NULL;
    xmlChar * nodePath = NULL;
    xmlChar * nodePathNew = NULL;

    *number = 0;

    /* Get node set for externaldata nodes */
    xpathContext = xmlXPathNewContext(aTixiDocument->docPtr);
    if (!xpathContext) {
        printMsg(MESSAGETYPE_ERROR, "Error: unable to create new XPath context\n");
        return FAILED;
    }

    xpathObject = xmlXPathEvalExpression((xmlChar *) "//externaldata", xpathContext);
    if (!xpathObject) {
        printMsg(MESSAGETYPE_ERROR, "Error: unable to to find externaldata nodes.\n");
        xmlXPathFreeContext(xpathContext);
        return INVALID_XPATH;
    }
    if (xmlXPathNodeSetIsEmpty(xpathObject->nodesetval)) {
        /* parent has no child at all, return child count 0 */
        *number = 0;
        xmlXPathFreeObject(xpathObject);
        xmlXPathFreeContext(xpathContext);
        return SUCCESS;
    }

    /* Now get through all "externaldata" nodes in node set */
    nodes = xpathObject->nodesetval;
    assert(nodes);
    for (iNode = 0; iNode < nodes->nodeNr; iNode++) {
        assert(nodes->nodeTab[iNode]);
        if (nodes->nodeTab[iNode]->type == XML_ELEMENT_NODE) {
            (*number)++;

            /* get nodes XPath */
            externalDataNodePath = (char *) xmlGetNodePath(nodes->nodeTab[iNode]);

            /* now get the subdirectory */
            externalDataDirectoryPath = (char *) malloc(sizeof(char) * strlen(externalDataNodePath) + strlen(EXTERNAL_DATA_NODE_NAME_PATH) + 4);
            externalDataDirectoryPath[0] = '\0';
            strcat(externalDataDirectoryPath, externalDataNodePath);
            strcat(externalDataDirectoryPath, "/");
            strcat(externalDataDirectoryPath, EXTERNAL_DATA_NODE_NAME_PATH);
            error = tixiGetTextElement(handle, externalDataDirectoryPath, &externalDataDirectory);
            free(externalDataDirectoryPath);
            if (error) {
                printMsg(MESSAGETYPE_ERROR, "Error: openExternalFiles returns %d when reading subpath.\n", error);
                return FAILED;
            }

            /* now get number and names of all external files */
            if (tixiGetNamedChildrenCount(handle, externalDataNodePath, EXTERNAL_DATA_NODE_NAME_FILENAME, &externalFileCount) != SUCCESS) {
                printMsg(MESSAGETYPE_ERROR, "Error: openExternalFiles could not get number of 'filename' children.\n");
                return FAILED;
            }

            /* iterate through all "filename" nodes */
             for (i = 1; i <= externalFileCount; i++) {
                ptrFilename = (char *) malloc(sizeof(char) * strlen(externalDataNodePath) + strlen(EXTERNAL_DATA_NODE_NAME_FILENAME) + 5);
                ptrFilename[0] = '\0';
                sprintf(ptrFilename, "%s/filename[%d]", externalDataNodePath, i);

                tixiGetTextElement(handle, ptrFilename, &externalFileName);
                free(ptrFilename);

                /* Build complete filename */
                externalFullFileName = (char *) malloc(sizeof(char) * strlen(externalFileName) + strlen(externalDataDirectory) + 1);
                sprintf(externalFullFileName, "%s%s", externalDataDirectory, externalFileName);

                /* open files */
                newDocumentString = curlGetURLInMemory(externalFullFileName);
                if (newDocumentString == NULL) {
                    printMsg(MESSAGETYPE_ERROR, "\nError in fetching url \"%s\".\n", externalFullFileName);
                    free(externalFullFileName);
                    return OPEN_FAILED;
                }
                free(externalFullFileName);
                xmlDocument = xmlReadMemory(newDocumentString, (int) strlen(newDocumentString), "urlResource", NULL, 0);
                free(newDocumentString);

                /* check the import */
                if (xmlDocument) {
                    //xmlNodePtr nodeToInsert;

                    /* save node informations to node-attribute */
                    /* save the filename */
                    nodePath = xmlGetNodePath(xmlDocGetRootElement(xmlDocument));

                    genericAddTextAttribute(xmlDocument, (char *) nodePath,
                            EXTERNAL_DATA_XML_ATTR_FILENAME, externalFileName);

                    /* save the sub-directory */
                    genericAddTextAttribute(xmlDocument, (char *) nodePath,
                            EXTERNAL_DATA_XML_ATTR_DIRECTORY, externalDataDirectory);

                    /* save the external data node position */
                    nodePathNew = xmlGetNodePath(getParentNodeToXPath(aTixiDocument->handle,
                                                                      externalDataNodePath));
                    genericAddTextAttribute(xmlDocument, (char *) nodePath,
                            EXTERNAL_DATA_XML_ATTR_NODEPATH, (char *) nodePathNew);

                    nodeToInsert = xmlDocCopyNode(xmlDocGetRootElement(xmlDocument), aTixiDocument->docPtr, 1);
                    xmlFreeDoc(xmlDocument);
                    xmlFree(nodePath);
                    xmlFree(nodePathNew);

                    /* merge trees */
                    xmlAddChildList(getParentNodeToXPath(handle, externalDataNodePath), nodeToInsert);
                } else {
                    return FAILED;
                }
            }
            /* remove externaldata-node */
            tixiRemoveElement(handle, externalDataNodePath);
            if (externalDataNodePath) {
                xmlFree(externalDataNodePath);
            }
            *number = externalFileCount;
        }
    }

    xmlXPathFreeContext(xpathContext);
    return error;
}


ReturnCode saveExternalFiles(xmlNodePtr aNodePtr, TixiDocument *aTixiDocument)
{
	TixiDocumentHandle handle = aTixiDocument->handle;
	xmlNodePtr cur_node = NULL;
	xmlNodePtr copiedNode = NULL;
	char *includetNodeName = NULL;
	char *externalDataDirectory = NULL;
	char *externalFileName = NULL;
	char *fullExternalFileName = NULL;
	char *externalDataNodePath = NULL;
	char *fullExternalDataNodePath = NULL;
	char *externalDataDirectoryNotUrl = NULL;
	xmlDocPtr xmlDocument = NULL;

	/* find external data nodes */
	for (cur_node = aNodePtr; cur_node; cur_node = cur_node->next) {

		/* recurse down with the next element */
		saveExternalFiles(cur_node->children, aTixiDocument);

		if( checkExternalNode( cur_node ) != SUCCESS) {
			continue;
		}

		if ( cur_node->type == XML_ELEMENT_NODE ){
			includetNodeName = (char *) malloc(sizeof(char) * strlen((char *) xmlGetNodePath(cur_node)) + 2);
			includetNodeName[0] = '\0';
			strcat(includetNodeName, (char *) xmlGetNodePath(cur_node));

			/* collect node information - externalFileName */
			tixiGetTextAttribute(handle, (char *) xmlGetNodePath(cur_node),
								 EXTERNAL_DATA_XML_ATTR_FILENAME, &externalFileName);

			/* collect node information - externalDataDirectory */
			tixiGetTextAttribute(handle, includetNodeName,
								 EXTERNAL_DATA_XML_ATTR_DIRECTORY, &externalDataDirectory);

			/* collect node information - externalDataNodePath */
			tixiGetTextAttribute(handle, includetNodeName,
								 EXTERNAL_DATA_XML_ATTR_NODEPATH, &externalDataNodePath);


			/* remove attributes */
			xmlUnsetProp(cur_node, (xmlChar *) EXTERNAL_DATA_XML_ATTR_FILENAME);
			xmlUnsetProp(cur_node, (xmlChar *) EXTERNAL_DATA_XML_ATTR_DIRECTORY);
			xmlUnsetProp(cur_node, (xmlChar *) EXTERNAL_DATA_XML_ATTR_NODEPATH);

			/* deep copy of nodes from external files */
			copiedNode = xmlCopyNodeList(cur_node);

			/* create new document */
			xmlDocument = xmlNewDoc((xmlChar *) "1.0");
			if (!xmlDocument) {
				printMsg(MESSAGETYPE_ERROR, "Error in TIXI::saveExternalFiles ==> Could not create new document.\n");
				return FAILED;
			}
			xmlDocSetRootElement(xmlDocument, copiedNode);

			/* save external file is it is not absolute*/
			if(isPathRelative(externalDataDirectory) == 0)
			{
			    /* strip "file://" from the beginning of the string to get the relative path */
			    externalDataDirectoryNotUrl = string_stripLeft(externalDataDirectory, 7);
			    if(strlen(externalDataDirectoryNotUrl)>1) {
                    create_local_directory(externalDataDirectoryNotUrl);
			    }
				fullExternalFileName = (char *) malloc(sizeof(char) * strlen(externalFileName) + strlen(externalDataDirectoryNotUrl) + 1);
				sprintf(fullExternalFileName, "%s%s", externalDataDirectoryNotUrl, externalFileName);
				xmlSaveFormatFileEnc(fullExternalFileName, xmlDocument, "utf-8", 1);
				free(fullExternalFileName);
			}

			/* create external data node structure */
			fullExternalDataNodePath = (char *) malloc(sizeof(char) * strlen(externalDataNodePath) + strlen(EXTERNAL_DATA_NODE_NAME) + 3);
			fullExternalDataNodePath[0] = '\0';
			strcat(fullExternalDataNodePath, externalDataNodePath);
			strcat(fullExternalDataNodePath, "/");
			strcat(fullExternalDataNodePath, EXTERNAL_DATA_NODE_NAME);

			/* add parent node if not exists */
			if(tixiCheckElement(handle, fullExternalDataNodePath) != SUCCESS) {
				tixiAddTextElement(handle, externalDataNodePath, EXTERNAL_DATA_NODE_NAME, "");
				tixiAddTextElement(handle, fullExternalDataNodePath, EXTERNAL_DATA_NODE_NAME_PATH, externalDataDirectory);
			}

			/* add node for external reference */
			tixiAddTextElement(handle, fullExternalDataNodePath, EXTERNAL_DATA_NODE_NAME_FILENAME, externalFileName);

			/* remove the copied nodes from document*/
			copiedNode = cur_node->prev;
			xmlUnlinkNode(cur_node);
			xmlFreeNode(cur_node);
			cur_node = copiedNode;
		}
	}
	return SUCCESS;
}



xmlNodePtr getParentNodeToXPath(TixiDocumentHandle handle, const char *elementPath)
{

  TixiDocument *document = getDocument(handle);
  xmlDocPtr xmlDocument = NULL;
  xmlXPathContextPtr xpathContext = NULL;
  xmlXPathObjectPtr xpathObject = NULL;
  xmlNodeSetPtr nodes = NULL;
  xmlNodePtr parent = NULL;


  if (!document) {
    printMsg(MESSAGETYPE_ERROR, "Error: Invalid document handle.\n");
    return parent;
  }

  xmlDocument = document->docPtr;

  /* Check parent element */
  xpathContext = xmlXPathNewContext(xmlDocument);

  if (!xpathContext) {
    printMsg(MESSAGETYPE_ERROR, "Error: unable to create new XPath context\n");
    xmlXPathFreeContext(xpathContext);
    return parent;
  }

  xpathObject = xmlXPathEvalExpression((xmlChar *) elementPath, xpathContext);

  if (!xpathObject) {
    printMsg(MESSAGETYPE_ERROR, "Error: unable to evaluate xpath expression \"%s\"\n", elementPath);
    xmlXPathFreeContext(xpathContext);
    return parent;
  }

  if (xmlXPathNodeSetIsEmpty(xpathObject->nodesetval)) {
    printMsg(MESSAGETYPE_ERROR, "Error: No element found at XPath expression \"%s\"\n", elementPath);
    xmlXPathFreeContext(xpathContext);
    xmlXPathFreeObject(xpathObject);
    return parent;
  }

  nodes = xpathObject->nodesetval;

  assert(nodes);

  if (nodes->nodeNr > 1) {
    printMsg(MESSAGETYPE_ERROR,
            "Error: Element chosen by XPath \"%s\" expression is not unique. \n", elementPath);
    xmlXPathFreeContext(xpathContext);
    xmlXPathFreeObject(xpathObject);
    return parent;
  }

  parent = nodes->nodeTab[0]->parent;

  xmlXPathFreeContext(xpathContext);
  xmlXPathFreeObject(xpathObject);
  return parent;
}


ReturnCode genericAddTextAttribute(xmlDocPtr xmlDocument, const char *elementPath,
                                const char *attributeName, const char *attributeValue)
{
  ReturnCode error = -1;
  xmlXPathObjectPtr xpathObject = NULL;
  xmlNodePtr parent = NULL;
  xmlAttrPtr attributePtr = NULL;

  if (!attributeName) {
    printMsg(MESSAGETYPE_ERROR, "Error:  No attribute name specified.\n");
    return NO_ATTRIBUTE_NAME;
  }

  if (!xmlValidateNameValue((xmlChar *) attributeName)) {
	  printMsg(MESSAGETYPE_ERROR, "Error: Invalid element name \"%s\"\n", attributeName);
	  return INVALID_XML_NAME;
  }

  error = checkElement(xmlDocument, elementPath, &parent, &xpathObject);
  if (!error) {
	  attributePtr = xmlSetProp(parent, (xmlChar *) attributeName, (xmlChar *) attributeValue);

	  if (!attributePtr) {
		  printMsg(MESSAGETYPE_ERROR,
			  "Error: Failed to add attribute \"%s\" to element \"%s\".\n",
			  attributeName, attributeValue);
		  xmlXPathFreeObject(xpathObject);
		  return FAILED;
	  }
	  xmlXPathFreeObject(xpathObject);
	  return SUCCESS;
  }
  else {
	  return error;
  }
}



ReturnCode getNodePtrFromElementPath(TixiDocumentHandle handle, const char *elementPath, xmlNodePtr *pNodePrt)
{
  TixiDocument *document = getDocument(handle);
  xmlDocPtr xmlDocument = NULL;
  xmlXPathContextPtr xpathContext = NULL;
  xmlXPathObjectPtr xpathObject = NULL;
  xmlNodeSetPtr nodes = NULL;

  if (!document) {
    printMsg(MESSAGETYPE_ERROR, "Error: Invalid document handle.\n");
    return INVALID_HANDLE;
  }

  xmlDocument = document->docPtr;

  xpathContext = xmlXPathNewContext(xmlDocument);

  if (!xpathContext) {
    printMsg(MESSAGETYPE_ERROR, "Error: unable to create new XPath context\n");
    xmlXPathFreeContext(xpathContext);
    return FAILED;
  }

  xpathObject = xmlXPathEvalExpression((xmlChar *) elementPath, xpathContext);

  if (!xpathObject) {
    printMsg(MESSAGETYPE_ERROR, "Error: unable to evaluate xpath expression \"%s\"\n", elementPath);
    xmlXPathFreeContext(xpathContext);
    return INVALID_XPATH;
  }

  if (xmlXPathNodeSetIsEmpty(xpathObject->nodesetval)) {
    printMsg(MESSAGETYPE_ERROR, "Error: No element found at XPath expression \"%s\"\n", elementPath);
    xmlXPathFreeContext(xpathContext);
    xmlXPathFreeObject(xpathObject);
    return ELEMENT_NOT_FOUND;
  }

  nodes = xpathObject->nodesetval;

  assert(nodes);

  if (nodes->nodeNr > 1) {
    printMsg(MESSAGETYPE_ERROR,
            "Error: Element chosen by XPath \"%s\" expression is not unique. \n", elementPath);
    xmlXPathFreeContext(xpathContext);
    xmlXPathFreeObject(xpathObject);
    return ELEMENT_PATH_NOT_UNIQUE;
  }

  *pNodePrt = nodes->nodeTab[0];
  xmlXPathFreeContext(xpathContext);
  xmlXPathFreeObject(xpathObject);

  return SUCCESS;
}


int copyDocument(const TixiDocumentHandle oldTixiDocumentHandle, TixiDocumentHandle *newTixiDocumentHandle)
{
	TixiDocument *srcDocument = getDocument(oldTixiDocumentHandle);
    TixiDocument *dstDocument = NULL;
    xmlDocPtr xmlDocument = NULL;
    xmlNodePtr rootNode = NULL;

    if (!srcDocument) {
        printMsg(MESSAGETYPE_ERROR, "Error in TIXI::copyDocument => Invalid document handle.\n");
        return FAILED;
    }

    /* make a deep copy of the document */
    xmlDocument = xmlCopyDoc(srcDocument->docPtr, 1);

    if (!xmlDocument) {
        printMsg(MESSAGETYPE_ERROR, "Error in TIXI::copyDocument => Could not copy document.\n");
        return FAILED;
    }

    rootNode = xmlDocGetRootElement(xmlDocument);
    if (!rootNode) {
        printMsg(MESSAGETYPE_ERROR, "Error in TIXI::copyDocument => Could not get root node in source document.\n");
        return EROROR_CREATE_ROOT_NODE;
    }

    dstDocument = (TixiDocument *) malloc(sizeof(TixiDocument));

    /* copy attributes from source document */
    if (srcDocument->xmlFilename != NULL) {
        dstDocument->xmlFilename = (char *) malloc(sizeof(char) * strlen(srcDocument->xmlFilename) + 1);
        strcpy(dstDocument->xmlFilename, srcDocument->xmlFilename);
    } else {
        dstDocument->xmlFilename = NULL;
    }

    if (srcDocument->dirname != NULL) {
        dstDocument->dirname = (char *) malloc(sizeof(char) * strlen(srcDocument->dirname) + 1);
        strcpy(dstDocument->dirname, srcDocument->dirname);
    } else {
        dstDocument->dirname = NULL;
    }

    if (srcDocument->filename != NULL) {
        dstDocument->filename = (char *) malloc(sizeof(char) * strlen(srcDocument->filename) + 1);
        strcpy(dstDocument->filename, srcDocument->filename);
    } else {
        dstDocument->filename = NULL;
    }

    if (srcDocument->validationFilename != NULL) {
        dstDocument->validationFilename = (char *) malloc(sizeof(char) * strlen(srcDocument->validationFilename) + 1);
        strcpy(dstDocument->validationFilename, srcDocument->validationFilename);
    } else {
        dstDocument->validationFilename = NULL;
    }

    dstDocument->docPtr = xmlDocument;
    dstDocument->currentNode = rootNode;
    dstDocument->isValid = srcDocument->isValid;
    dstDocument->status = srcDocument->status;
    dstDocument->memoryListHead = NULL;
    dstDocument->memoryListTail = NULL;
    dstDocument->uidListHead = NULL;
    dstDocument->hasIncludedExternalFiles = srcDocument->hasIncludedExternalFiles;

    if (addDocumentToList(dstDocument, &(dstDocument->handle)) != SUCESS) {
        printMsg(MESSAGETYPE_ERROR, "Error in TIXI::copyDocument => Failed  adding document to document list.");
        return FAILED;
    }

    *newTixiDocumentHandle = dstDocument->handle;
    return SUCCESS;
}


ReturnCode saveDocument (TixiDocumentHandle handle, const char *xmlFilename, InternalSaveMode saveMode)
{
	TixiDocument *document = getDocument(handle);
    TixiDocument *cpyDoc = NULL;
    xmlNodePtr rootNode = NULL;
    TixiDocumentHandle newHandle;

    if (!xmlFilename) {
        printMsg(MESSAGETYPE_ERROR, "Error: No filename given.\n");
        return FAILED;
    }

    if (!document) {
        printMsg(MESSAGETYPE_ERROR, "Error: Invalid document handle.\n");
        return INVALID_HANDLE;
    }

    if (saveMode == COMPLETE) {
        if (xmlSaveFormatFileEnc(xmlFilename, document->docPtr, "utf-8", document->usePrettyPrint) == -1) {
            printMsg(MESSAGETYPE_ERROR, "Error: Failed in writing document to file.\n");
            return FAILED;
        };

    } else if (saveMode == SPLITTED) {
        /* save included nodes back to external files */
        /* first create a copy and don't chance the original document */
        copyDocument(handle, &newHandle);
        cpyDoc = getDocument(newHandle);
        rootNode = xmlDocGetRootElement(cpyDoc->docPtr);
        saveExternalFiles(rootNode, cpyDoc);

        if (xmlSaveFormatFileEnc(xmlFilename, cpyDoc->docPtr, "utf-8", document->usePrettyPrint) == -1) {
            printMsg(MESSAGETYPE_ERROR, "Error: Failed in writing document to file.\n");
            return FAILED;
        };
        removeDocumentFromList(newHandle);
        freeTixiDocument(cpyDoc);

    } else if (saveMode == REMOVED) {
        rootNode = xmlDocGetRootElement(document->docPtr);
        saveExternalFiles(rootNode, document);

        if (xmlSaveFormatFileEnc(xmlFilename, document->docPtr, "utf-8", document->usePrettyPrint) == -1) {
            printMsg(MESSAGETYPE_ERROR, "Error: Failed in writing document to file.\n");
            return FAILED;
        };

    } else {
        return FAILED;
    }

    free(document->xmlFilename);
    document->xmlFilename = (char *) malloc(sizeof(char) * (strlen(xmlFilename) + 1));
    strcpy(document->xmlFilename, xmlFilename);

    document->status = SAVED;
    return SUCCESS;
}



ReturnCode validateSchema(const TixiDocumentHandle handle, xmlDocPtr *schema_doc)
{
    TixiDocument *document = getDocument(handle);
    xmlSchemaParserCtxtPtr parser_ctxt;
    xmlSchemaPtr schema;
    xmlSchemaValidCtxtPtr valid_ctxt;
    int is_valid;

    if (*schema_doc == NULL) {
        /* the schema cannot be loaded or is not well-formed */
        return OPEN_SCHEMA_FAILED;
    }
    parser_ctxt = xmlSchemaNewDocParserCtxt(*schema_doc);
    if (parser_ctxt == NULL) {
        printMsg(MESSAGETYPE_ERROR, "Error: validateSchema: unable to create a parser context for the schema.\n");
        xmlFreeDoc(*schema_doc);
        return FAILED;
    }
    schema = xmlSchemaParse(parser_ctxt);
    if (schema == NULL) {
        printMsg(MESSAGETYPE_ERROR, "Error: validateSchema: the schema itself is not valid.\n");
        xmlSchemaFreeParserCtxt(parser_ctxt);
        xmlFreeDoc(*schema_doc);
        return FAILED;
    }
    valid_ctxt = xmlSchemaNewValidCtxt(schema);
    if (valid_ctxt == NULL) {
        printMsg(MESSAGETYPE_ERROR, "Error: validateSchema: unable to create a validation context for the schema.\n");
        xmlSchemaFree(schema);
        xmlSchemaFreeParserCtxt(parser_ctxt);
        xmlFreeDoc(*schema_doc);
        return FAILED;
    }
    is_valid = (xmlSchemaValidateDoc(valid_ctxt, document->docPtr) == 0);
    xmlSchemaFreeValidCtxt(valid_ctxt);
    xmlSchemaFree(schema);
    xmlSchemaFreeParserCtxt(parser_ctxt);
    xmlFreeDoc(*schema_doc);

    if (is_valid) {
        return SUCCESS;
    } else {
        return NOT_SCHEMA_COMPLIANT;
    }
}

char* generateXPathFromNodePtr(TixiDocumentHandle handle, xmlNodePtr aNodePtr)
{
    xmlNodePtr nodePtr = aNodePtr;
    TixiDocument *document = getDocument(handle);
    char *generatedXPath = NULL;
    char *textPtr = NULL;
    char *tmpText = NULL;
    char *nodeName = NULL;
    size_t size = 0;
    generatedXPath = malloc(1 * sizeof(char));
    generatedXPath[0] = '\0';

    if (!document) {
        printMsg(MESSAGETYPE_ERROR, "Error: Invalid document handle.\n");
        free(generatedXPath);
        return NULL;
    }

    while ((nodePtr != NULL) && (nodePtr->parent != NULL)) {
        if (nodePtr->type == XML_ELEMENT_NODE) {

            /* handle uIDs */
            textPtr = (char *) xmlGetProp(nodePtr, (xmlChar *) "uID");
            if (textPtr) {
                // do something with the attribute...
                size = strlen((char*) nodePtr->name) + strlen(textPtr) + strlen("[@uID=\"\"]") + 1;
                nodeName = (char *) malloc(size * sizeof(char));
                sprintf(nodeName, "%s[@uID=\"%s\"]", nodePtr->name, textPtr);
                xmlFree(textPtr);

            } else {
                nodeName = (char *) malloc((strlen((char*) nodePtr->name) + 1) * sizeof(char));
                strcpy(nodeName, (char*) nodePtr->name);
            }

          tmpText = (char *) malloc((strlen(generatedXPath) + 1) * sizeof(char));
          strcpy(tmpText, generatedXPath);
          free(generatedXPath);
          generatedXPath = (char *) malloc((strlen(tmpText) + strlen(nodeName) + 2) * sizeof(char));    // + slash + null
          sprintf(generatedXPath, "/%s%s", nodeName, tmpText);
          free(tmpText);
          free(nodeName);
      }
      nodePtr = nodePtr->parent;
    }

    addToMemoryList(document, (void *) generatedXPath);
    return generatedXPath;
}

