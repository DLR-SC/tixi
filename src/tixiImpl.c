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
 * @file   tixiImpl.c
 * @author Hans-Peter Kersken <Hans-Peter.Kersken@dlr.de>
 * @date   Mon Mar 14 16:19:56 2005
 * @brief  Implementation of the TIXI-C-API
 * @todo add not an Element to tixiGetTextAttribute
 */

#include <assert.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include "libxml/parser.h"
#include "libxml/xpath.h"
#include "libxml/xmlschemas.h"

#include "tixiInternal.h"
#include "xslTransformation.h"
#include "webMethods.h"
#include "uidHelper.h"
#include "xpathFunctions.h"
#include "tixiUtils.h"
#include "tixi.h"
#include "tixi_version.h"
#include "namespaceFunctions.h"

static xmlNsPtr nameSpace = NULL;

void tixiDefaultMessageHandler(MessageType type, const char* msg);

/**
  Pointer to the head of the list of documents managed by TIXI
*/
TixiDocumentListEntry *documentListHead = NULL;

TixiPrintMsgFnc tixiMessageHandler = tixiDefaultMessageHandler;

/**
  Route all messages to the internal message handler
 */
void printMsg(MessageType type, const char* message, ...)
{
  char buffer[2048];
  char* extra = NULL;
  int len = 0;

  va_list varArgs;
  va_start(varArgs, message);
  if ((len = vsnprintf(buffer, 2048, message, varArgs)) >= 2048) {
    // message is longer than 2048 bytes,
    // we must allocate
    extra = (char*) malloc((len+1) * sizeof(char));

    vsnprintf(extra, len+1, message, varArgs);
    if (tixiMessageHandler) {
      tixiMessageHandler(type, extra);
    }

    if (extra) {
      free(extra);
    }
  }
  else {
    if (tixiMessageHandler) {
      tixiMessageHandler(type, buffer);
    }
  }

  va_end(varArgs);
}

int _initialized = 0;

void xmlErrorHandler(void * ctx, const char *message, ...) {
  char buffer[2048];
  char* extra = NULL;
  int len = 0;

  va_list varArgs;
  va_start(varArgs, message);
  if ((len = vsnprintf(buffer, 2048, message, varArgs)) >= 2048) {
    // message is longer than 2048 bytes,
    // we must allocate
    extra = (char*) malloc((len+1) * sizeof(char));

    vsnprintf(extra, len+1, message, varArgs);
    tixiMessageHandler(MESSAGETYPE_ERROR, extra);

    if (extra) {
      free(extra);
    }
  }
  else {
    tixiMessageHandler(MESSAGETYPE_ERROR, buffer);
  }

  va_end(varArgs);
}

static void tixiInit(void)
{
  if (!_initialized) {
    printMsg(MESSAGETYPE_STATUS, "TiXI initialized\n");
    xmlSetGenericErrorFunc(NULL, xmlErrorHandler);
    xsltSetGenericErrorFunc(NULL, xmlErrorHandler);
    _initialized = 1;
  }
}


/**
  gives the tixi version number
*/
DLL_EXPORT char* tixiGetVersion()
{
  static char version[] = TIXI_VERSION_STRING;
  return version;
}


/**
  Prints out an error code with a small explanation of the error.
*/
void printError(ReturnCode error, char *function)
{
  switch(error)
  {
  case SUCCESS:
    printMsg(MESSAGETYPE_ERROR, "Error: %s returns %d. No error occurred.\n", function, error);
    break;
  case FAILED:
    printMsg(MESSAGETYPE_ERROR, "Error: %s returns %d. Unspecified error .\n", function, error);
    break;
  case INVALID_XML_NAME:
    printMsg(MESSAGETYPE_ERROR, "Error: %s returns %d. Non XML standard complaint name specified.\n", function, error);
    break;
  case NOT_WELL_FORMED:
    printMsg(MESSAGETYPE_ERROR, "Error: %s returns %d. Document is not well formed.\n", function, error);
    break;
  case NOT_SCHEMA_COMPLIANT:
    printMsg(MESSAGETYPE_ERROR, "Error: %s returns %d. Document is not schema complaint.\n", function, error);
    break;
  case NOT_DTD_COMPLIANT:
    printMsg(MESSAGETYPE_ERROR, "Error: %s returns %d. Document is not DTD complaint.\n", function, error);
    break;
  case INVALID_HANDLE:
    printMsg(MESSAGETYPE_ERROR, "Error: %s returns %d. Document handle is not valid.\n", function, error);
    break;
  case INVALID_XPATH:
    printMsg(MESSAGETYPE_ERROR, "Error: %s returns %d. XPath expression is not valid.\n", function, error);
    break;
  case ELEMENT_NOT_FOUND:
    printMsg(MESSAGETYPE_ERROR, "Error: %s returns %d. Element does not exist in document.\n", function, error);
    break;
  case INDEX_OUT_OF_RANGE:
    printMsg(MESSAGETYPE_ERROR, "Error: %s returns %d. Index supplied as argument is not inside the admissible range.\n", function, error);
    break;
  case NO_POINT_FOUND:
    printMsg(MESSAGETYPE_ERROR, "Error: %s returns %d. No point element found a given XPath.\n", function, error);
    break;
  case NOT_AN_ELEMENT:
    printMsg(MESSAGETYPE_ERROR, "Error: %s returns %d. XPath expression does not point to an XML-element node .\n", function, error);
    break;
  case ATTRIBUTE_NOT_FOUND:
    printMsg(MESSAGETYPE_ERROR, "Error: %s returns %d. Element does not have the attribute .\n", function, error);
    break;
  case OPEN_FAILED:
    printMsg(MESSAGETYPE_ERROR, "Error: %s returns %d. Error on opening the file   .\n", function, error);
    break;
  case OPEN_SCHEMA_FAILED:
    printMsg(MESSAGETYPE_ERROR, "Error: %s returns %d. Error on opening the schema file.\n", function, error);
    break;
  case OPEN_DTD_FAILED:
    printMsg(MESSAGETYPE_ERROR, "Error: %s returns %d. Error on opening the DTD file.\n", function, error);
    break;
  case CLOSE_FAILED:
    printMsg(MESSAGETYPE_ERROR, "Error: %s returns %d. Error on closing the file.\n", function, error);
    break;
  case ALREADY_SAVED:
    printMsg(MESSAGETYPE_ERROR, "Error: %s returns %d. Trying to modify already saved document.\n", function, error);
    break;
  case ELEMENT_PATH_NOT_UNIQUE:
    printMsg(MESSAGETYPE_ERROR, "Error: %s returns %d. Path expression can not be resolved unambiguously.\n", function, error);
    break;
  case NO_ELEMENT_NAME:
    printMsg(MESSAGETYPE_ERROR, "Error: %s returns %d. Element name argument is NULL .\n", function, error);
    break;
  case NO_CHILDREN:
    printMsg(MESSAGETYPE_ERROR, "Error: %s returns %d. Node has no children.\n", function, error);
    break;
  case CHILD_NOT_FOUND:
    printMsg(MESSAGETYPE_ERROR, "Error: %s returns %d. Named child is not child of element specified .\n", function, error);
    break;
  case EROROR_CREATE_ROOT_NODE:
    printMsg(MESSAGETYPE_ERROR, "Error: %s returns %d. Error when adding root node to new document.\n", function, error);
    break;
  case DEALLOCATION_FAILED:
    printMsg(MESSAGETYPE_ERROR, "Error: %s returns %d. On closing a document the deallocation of allocated memory fails.\n", function, error);
    break;
  case NO_NUMBER:
    printMsg(MESSAGETYPE_ERROR, "Error: %s returns %d. No number specified.\n", function, error);
    break;
  case NO_ATTRIBUTE_NAME:
    printMsg(MESSAGETYPE_ERROR, "Error: %s returns %d. No attribute name specified.\n", function, error);
    break;
  case STRING_TRUNCATED:
    printMsg(MESSAGETYPE_ERROR, "Error: %s returns %d. String variable supplied is to small to hold the result, Fortran only.\n", function, error);
    break;
  case NON_MATCHING_NAME:
    printMsg(MESSAGETYPE_ERROR, "Error: %s returns %d. Row or column name specified do not match the names used in the document.\n", function, error);
    break;
  case NON_MATCHING_SIZE:
    printMsg(MESSAGETYPE_ERROR, "Error: %s returns %d. Number of rows or columns specified do not match the sizes of the matrix in the document .\n", function, error);
    break;
  case MATRIX_DIMENSION_ERROR:
    printMsg(MESSAGETYPE_ERROR, "Error: %s returns %d. If nRows or nColumns or both are less than 1.\n", function, error);
    break;
  case COORDINATE_NOT_FOUND:
    printMsg(MESSAGETYPE_ERROR, "Error: %s returns %d. Missing coordinate inside a point element.\n", function, error);
    break;
  case UNKNOWN_STORAGE_MODE:
    printMsg(MESSAGETYPE_ERROR, "Error: %s returns %d. storage mode specified is neither ROW_WISE nor COLUMN_WISE.\n", function, error);
    break;
  case UID_NOT_UNIQUE:
    printMsg(MESSAGETYPE_ERROR, "Error: %s returns %d. One or more uID's are not unique.\n", function, error);
    break;
  case UID_DONT_EXISTS:
    printMsg(MESSAGETYPE_ERROR, "Error: %s returns %d. A given uID's does not exist.\n", function, error);
    break;
  case UID_LINK_BROKEN:
    printMsg(MESSAGETYPE_ERROR, "Error: %s returns %d. .\n", function, error);
    break;
  default:
    printMsg(MESSAGETYPE_ERROR, "Error: %s returns %d. No further description specified.\n", function, error);
    break;
  }
}


/**
 *  Opens the file and sets up the TixiDocument datastructure.
 */
DLL_EXPORT ReturnCode tixiOpenDocumentRecursive(const char *xmlFilename, TixiDocumentHandle *handle, OpenMode oMode)
{
  /* this opens the XML-file and checks if it is well formed */

  TixiDocument *document = NULL;
  xmlDocPtr xmlDocument = NULL;
  FILE *file = NULL;
  ReturnCode returnValue = -1;

  tixiInit();
  checkLibxml2Version();

  xmlKeepBlanksDefault(0);
  xmlIndentTreeOutput = 1;

  assert(xmlFilename);

  file = fopen(xmlFilename, "r");
  if (!file) {
    printMsg(MESSAGETYPE_ERROR, "Error: Unable to open file \"%s\".\n", xmlFilename);
    return OPEN_FAILED;
  }
  else {
    fclose(file);
  }

  xmlDocument = xmlReadFile(xmlFilename, NULL, 0);

  if (xmlDocument) {

    document = (TixiDocument *) malloc(sizeof(TixiDocument));
    document->xmlFilename = (char *) malloc(strlen(xmlFilename) * sizeof(char) + 1);
    strcpy(document->xmlFilename, xmlFilename);
    strip_dirname(xmlFilename, &document->dirname, &document->filename);
    document->validationFilename = NULL;
    document->docPtr = xmlDocument;
    document->currentNode = NULL;
    document->isValid = UNDEFINED;
    document->status = OPENED;
    document->memoryListHead = NULL;
    document->memoryListTail = NULL;
    document->hasIncludedExternalFiles = 1;
    document->usePrettyPrint = 1;
    document->uidListHead = NULL;
    document->xpathContext = xmlXPathNewContext(xmlDocument);
    addDocumentToList(document, &(document->handle));
    *handle = document->handle;
    returnValue = SUCCESS; /*?*/

    if (oMode == OPENMODE_RECURSIVE) {
      int count = 0;
      document->hasIncludedExternalFiles = 1;

      returnValue = openExternalFiles(document, &count);
      if (returnValue != SUCCESS){
        printMsg(MESSAGETYPE_ERROR, "Error %d in including external files into tixiDoument.\n", returnValue);
        removeDocumentFromList(*handle);
        freeTixiDocument(document);
        document = NULL;
      }
    }
  }
  else {
    printMsg(MESSAGETYPE_ERROR, "Error: \"%s\" is not a wellformed XML-file.\n", xmlFilename);
    returnValue = NOT_WELL_FORMED;
  }
  return returnValue;
}


DLL_EXPORT ReturnCode tixiOpenDocument(const char *xmlFilename, TixiDocumentHandle *handle)
{
  return tixiOpenDocumentRecursive(xmlFilename, handle, OPENMODE_PLAIN);
}



DLL_EXPORT ReturnCode tixiOpenDocumentFromHTTP (const char *httpURL, TixiDocumentHandle *handle)
{
  char* xmlDocument = NULL;
  ReturnCode returnCode = FAILED;

  tixiInit();
  xmlKeepBlanksDefault(0);
  xmlIndentTreeOutput = 1;

  xmlDocument = curlGetURLInMemory(httpURL);
  if(xmlDocument == NULL) {
    return OPEN_FAILED;
  }

  returnCode = tixiImportFromString(xmlDocument, handle);
  free(xmlDocument);
  return returnCode;
}


DLL_EXPORT ReturnCode tixiCreateDocument(const char *rootElementName, TixiDocumentHandle *handle)
{
  TixiDocument *document = NULL;
  xmlDocPtr xmlDocument = NULL;
  xmlNodePtr rootNode = NULL;

  tixiInit();
  xmlKeepBlanksDefault(0);
  xmlIndentTreeOutput = 1;

  if (!rootElementName) {
    printMsg(MESSAGETYPE_ERROR, "Error: No root element name specified.\n");
    return NO_ELEMENT_NAME;
  }

  if (!xmlValidateNameValue((xmlChar *) rootElementName)) {
    printMsg(MESSAGETYPE_ERROR, "Error: Invalid name for root element \"%s\"\n", rootElementName);
    return INVALID_XML_NAME;
  }

  xmlDocument = xmlNewDoc((xmlChar *) "1.0");

  if (!xmlDocument) {
    printMsg(MESSAGETYPE_ERROR, "Error: Could not create new document.\n");
    return FAILED;
  }

  rootNode = xmlNewDocRawNode(xmlDocument, NULL, (xmlChar *) rootElementName, NULL);

  if (!rootNode) {
    printMsg(MESSAGETYPE_ERROR, "Error: Could not create root node \"%s\" in new document.\n", rootElementName);
    return EROROR_CREATE_ROOT_NODE;
  }

  xmlDocSetRootElement(xmlDocument, rootNode);

  document = (TixiDocument *) malloc(sizeof(TixiDocument));
  document->xmlFilename = NULL;
  document->dirname = NULL;
  document->filename = NULL;
  document->validationFilename = NULL;
  document->docPtr = xmlDocument;
  document->currentNode = rootNode;
  document->isValid = UNDEFINED;
  document->status = OPENED;
  document->memoryListHead = NULL;
  document->memoryListTail = NULL;
  document->hasIncludedExternalFiles = 1;
  document->usePrettyPrint = 1;
  document->uidListHead = NULL;
  document->xpathContext = xmlXPathNewContext(xmlDocument);

  if (addDocumentToList(document, &(document->handle)) != SUCESS) {
    printMsg(MESSAGETYPE_ERROR, "Error: Failed  adding document to document list.");
    return FAILED;
  }

  *handle = document->handle;
  return SUCCESS;
}

DLL_EXPORT ReturnCode tixiGetDocumentPath(TixiDocumentHandle handle, char** documentPath)
{
  TixiDocument *document = NULL;

  if (!documentPath) {
    printMsg(MESSAGETYPE_ERROR, "Error: Null Pointer in tixiGetDocumentPath.\n");
    return FAILED;
  }

  document = getDocument(handle);
  if (!document) {
    printMsg(MESSAGETYPE_ERROR, "Error: Invalid document handle in tixiGetDocumentPath.\n");
    return INVALID_HANDLE;
  }

  *documentPath = document->xmlFilename;

  return SUCCESS;
}

/*
Distinguish internal/user/fatal errors!!!
*/
DLL_EXPORT ReturnCode tixiSaveDocument(TixiDocumentHandle handle, const char *xmlFilename)
{
  return saveDocument(handle, xmlFilename, SPLITTED);
}



DLL_EXPORT ReturnCode tixiSaveCompleteDocument(TixiDocumentHandle handle, const char *xmlFilename)
{
  return saveDocument(handle, xmlFilename, COMPLETE);
}


DLL_EXPORT ReturnCode tixiSaveAndRemoveDocument (TixiDocumentHandle handle, const char *xmlFilename)
{
  return saveDocument(handle, xmlFilename, REMOVED);
}


DLL_EXPORT ReturnCode tixiCloseDocument(TixiDocumentHandle handle)
{
  TixiDocument *document = getDocument(handle);

  if (!document) {
    printMsg(MESSAGETYPE_ERROR, "Error: Invalid document handle in tixiCloseDocument.\n");
    return INVALID_HANDLE;
  }

  if (removeDocumentFromList(handle) == FAILED) {
    if (document) {
      freeTixiDocument(document);
      document = NULL;
    }
    return CLOSE_FAILED;
  }

  if (document) {
    freeTixiDocument(document);
    document = NULL;
  }

  return SUCCESS;
}

DLL_EXPORT ReturnCode tixiCloseAllDocuments()
{
  ReturnCode returnValue = SUCCESS;

  while(documentListHead && documentListHead->document) {
    TixiDocumentHandle handle = documentListHead->document->handle;
    returnValue = tixiCloseDocument(handle);
    if(returnValue != SUCCESS)
      return returnValue;
  }
  return SUCCESS;
}

DLL_EXPORT ReturnCode tixiCleanup()
{
  ReturnCode ret = tixiCloseAllDocuments();
  if(ret != SUCCESS)
    return ret;

  // cleanup libxml, removes valgrind leaks
  xmlCleanupParser();
  return SUCCESS;
}


DLL_EXPORT ReturnCode tixiExportDocumentAsString(const TixiDocumentHandle handle, char **text)
{
  TixiDocument *document = getDocument(handle);
  xmlDocPtr xmlDocument = NULL;
  xmlChar *xmlbuff;
  int buffersize;
  char *textPtr = NULL;
  ReturnCode error = SUCCESS;


  if (!document) {
    printMsg(MESSAGETYPE_ERROR, "Error: Invalid document handle.\n");
    return INVALID_HANDLE;
  }

  if (!text) {
    printMsg(MESSAGETYPE_ERROR, "Error: Null Pointer in tixiExportDocumentAsString.\n");
    return FAILED;
  }

  xmlDocument = document->docPtr;

  xmlDocDumpFormatMemory(xmlDocument, &xmlbuff, &buffersize, document->usePrettyPrint);
  textPtr = (char *) xmlbuff;

  if ( textPtr ) {
    *text = (char *) malloc((strlen(textPtr) + 1) * sizeof(char));
    strcpy(*text, textPtr);
    xmlFree(textPtr);
  }
  else {
    *text = (char *) malloc(sizeof(char));
    strcpy(*text, "");
  }
  error = addToMemoryList(document, (void *) *text);

  return error;
}


DLL_EXPORT ReturnCode tixiImportFromString (const char *xmlImportString, TixiDocumentHandle * handle)
{
  /* this imports a XML-string and checks if it is well formed */
  TixiDocument *document = NULL;
  xmlDocPtr xmlDocument = NULL;
  ReturnCode returnValue = -1;
  char *tixiFileName = NULL;
  char relativDirPath[3];

  tixiInit();
  tixiFileName = (char *) malloc(sizeof(char) * strlen("tixiDocument.xml") + 1);
  tixiFileName[0] = '\0';
  strcat(tixiFileName, "tixiDocument.xml");

  relativDirPath[0] = '\0';
  strcat(relativDirPath, "./");

  xmlDocument = xmlReadMemory(xmlImportString, (int) strlen(xmlImportString), tixiFileName, NULL, 0);

  if (xmlDocument) {

    document = (TixiDocument *) malloc(sizeof(TixiDocument));
    document->xmlFilename = NULL;
    document->dirname = NULL;
    document->filename = NULL;
    document->validationFilename = NULL;
    document->docPtr = xmlDocument;
    document->currentNode = NULL;
    document->isValid = UNDEFINED;
    document->status = OPENED;
    document->memoryListHead = NULL;
    document->memoryListTail = NULL;
    document->hasIncludedExternalFiles = 1;
    document->usePrettyPrint = 1;
    document->uidListHead = NULL;
    document->xpathContext = xmlXPathNewContext(xmlDocument);
    addDocumentToList(document, &(document->handle));
    *handle = document->handle;
    returnValue = SUCCESS; /*?*/
  }
  else {
    printMsg(MESSAGETYPE_ERROR, "Error: XML-string to import is not wellformed!\n");
    returnValue = NOT_WELL_FORMED;
  }
  free(tixiFileName);
  return returnValue;
}




DLL_EXPORT ReturnCode tixiAddHeader(const TixiDocumentHandle handle, const char *toolName, const char *version, const char *authorName)
{

  TixiDocument *document = getDocument(handle);
  xmlNodePtr headerElement = NULL;
  xmlNodePtr rootElement = NULL;



  if (!document) {
    printMsg(MESSAGETYPE_ERROR, "Error: Invalid document handle.\n");
    return INVALID_HANDLE;
  }

  if (document->status == SAVED) {
    printMsg(MESSAGETYPE_ERROR, "Error:  Can not add header to document. Document already saved.\n");
    return ALREADY_SAVED;
  }

  headerElement = xmlNewNode(nameSpace, (xmlChar *) "header");

  if (headerElement) {

    xmlNodePtr toolInfo = xmlNewTextChild(headerElement, nameSpace, (xmlChar *) "tool", NULL);

    xmlNodePtr authorElment = NULL;

    if (!toolInfo) {
      printMsg(MESSAGETYPE_ERROR, "Error:  Failed to created \"tool\" element.\n");
      return FAILED;
    }
    else {
      xmlNodePtr newChild = xmlNewTextChild(toolInfo, nameSpace, (xmlChar *) "toolName",
                                            (xmlChar *) toolName);

      if (!newChild) {
        printMsg(MESSAGETYPE_ERROR, "Error:  Failed to created \"toolName\" element.\n");
        return FAILED;
      }
      newChild = xmlNewTextChild(toolInfo, nameSpace, (xmlChar *) "version", (xmlChar *) version);
      if (!newChild) {
        printMsg(MESSAGETYPE_ERROR, "Error:  Failed to created version\" element.\n");
        return FAILED;
      }
    }

    authorElment =
        xmlNewTextChild(headerElement, nameSpace, (xmlChar *) "author", (xmlChar *) authorName);

    if (!authorElment) {
      printMsg(MESSAGETYPE_ERROR, "Error:  Failed to created \"author\" element.\n");
      return FAILED;
    }

    /* Add date string */
    {
      time_t now = time(NULL);
      struct tm *date = localtime(&now);
      char timestamp[65];
      xmlNodePtr newChild = NULL;

      sprintf(timestamp, "%02d-%02d-%02dT%02d:%02d:%02d",
              date->tm_year + 1900, date->tm_mon + 1, date->tm_mday,
              date->tm_hour, date->tm_min, date->tm_sec);

      newChild =
          xmlNewTextChild(headerElement, nameSpace, (xmlChar *) "timestamp", (xmlChar *) timestamp);

      if (!newChild) {
        printMsg(MESSAGETYPE_ERROR, "Error:  Failed to add timestamp element.\n");
        return FAILED;
      }
    }
  }
  else {
    printMsg(MESSAGETYPE_ERROR, "Error:  Failed to created \"tixiHeader\" element.\n");
    return FAILED;
  }

  rootElement = xmlDocGetRootElement(document->docPtr);
  if (!rootElement) {
    printMsg(MESSAGETYPE_ERROR, "Error:  Failed to get root element.\n");
    return FAILED;
  }

  if (!xmlAddChild(rootElement, headerElement)) {
    printMsg(MESSAGETYPE_ERROR, "Error:  Failed to add header element.\n");
    return FAILED;
  }
  return SUCCESS;
}


DLL_EXPORT ReturnCode tixiAddCpacsHeader(const TixiDocumentHandle handle,
                                         const char *name, const char *creator, const char *version,
                                         const char *description, const char * cpacsVersion)
{
  TixiDocument *document = getDocument(handle);
  xmlNodePtr headerElement = NULL;
  xmlNodePtr rootElement = NULL;



  if (!document) {
    printMsg(MESSAGETYPE_ERROR, "Error: Invalid document handle.\n");
    return INVALID_HANDLE;
  }

  if (document->status == SAVED) {
    printMsg(MESSAGETYPE_ERROR,
             "Error:  Can not add header to document. Document already saved.\n");
    return ALREADY_SAVED;
  }

  headerElement = xmlNewNode(nameSpace, (xmlChar *) "header");

  if (headerElement) {

    xmlNodePtr creatorElement = NULL;

    xmlNodePtr descriptionElement = NULL;

    xmlNodePtr newChild = xmlNewTextChild(headerElement, nameSpace, (xmlChar *) "name", (xmlChar *) name);

    xmlNodePtr cpacsVersionElement = NULL;

    if (!newChild) {
      printMsg(MESSAGETYPE_ERROR, "Error:  Failed to create \"toolName\" element.\n");
      return FAILED;
    }

    newChild = xmlNewTextChild(headerElement, nameSpace,
                               (xmlChar *) "version", (xmlChar *) version);
    if (!newChild) {
      printMsg(MESSAGETYPE_ERROR, "Error:  Failed to create version\" element.\n");
      return FAILED;
    }

    cpacsVersionElement = xmlNewTextChild(headerElement, nameSpace,
                                          (xmlChar *) "cpacsVersion", (xmlChar *) cpacsVersion);
    if (!cpacsVersionElement) {
      printMsg(MESSAGETYPE_ERROR, "Error:  Failed to create cpacs version\" element.\n");
      return FAILED;
    }

    creatorElement = xmlNewTextChild(headerElement, nameSpace,
                                     (xmlChar *) "creator", (xmlChar *) creator);
    if (!creatorElement) {
      printMsg(MESSAGETYPE_ERROR, "Error:  Failed to create \"creator\" element.\n");
      return FAILED;
    }

    descriptionElement = xmlNewTextChild(headerElement, nameSpace,
                                         (xmlChar *) "description", (xmlChar *) description);
    if (!descriptionElement) {
      printMsg(MESSAGETYPE_ERROR,
               "Error:  Failed to create \"description\" element.\n");
      return FAILED;
    }

    /* Add date string */
    {
      time_t now = time(NULL);
      struct tm *date = localtime(&now);
      char timestamp[65];
      xmlNodePtr newChild = NULL;

      sprintf(timestamp, "%02d-%02d-%02dT%02d:%02d:%02d", date->tm_year
              + 1900, date->tm_mon + 1, date->tm_mday, date->tm_hour,
              date->tm_min, date->tm_sec);

      newChild = xmlNewTextChild(headerElement, nameSpace,
                                 (xmlChar *) "timestamp", (xmlChar *) timestamp);

      if (!newChild) {
        printMsg(MESSAGETYPE_ERROR, "Error:  Failed to add timestamp element.\n");
        return FAILED;
      }
    }
  }

  rootElement = xmlDocGetRootElement(document->docPtr);
  if (!rootElement) {
    printMsg(MESSAGETYPE_ERROR, "Error:  Failed to get root element.\n");
    return FAILED;
  }

  if (!xmlAddChild(rootElement, headerElement)) {
    printMsg(MESSAGETYPE_ERROR, "Error:  Failed to add header element.\n");
    return FAILED;
  }
  return SUCCESS;
}


DLL_EXPORT ReturnCode tixiSchemaValidateFromFile(const TixiDocumentHandle handle, const char *xsdFilename)
{
  xmlDocPtr schema_doc;

  schema_doc = xmlReadFile(xsdFilename, NULL, XML_PARSE_NONET);
  return( validateSchema(handle, &schema_doc, 0));
}

DLL_EXPORT ReturnCode tixiSchemaValidateWithDefaultsFromFile(const TixiDocumentHandle handle, const char *xsdFilename)
{
  xmlDocPtr schema_doc;

  schema_doc = xmlReadFile(xsdFilename, NULL, XML_PARSE_NONET);
  return( validateSchema(handle, &schema_doc, 1));
}

DLL_EXPORT ReturnCode tixiSchemaValidateFromString(const TixiDocumentHandle handle, const char *xsdString)
{
  xmlDocPtr schema_doc;

  schema_doc = xmlReadMemory(xsdString, (int) strlen(xsdString), NULL, NULL, 0);
  return(validateSchema(handle, &schema_doc, 0));
}


DLL_EXPORT ReturnCode tixiDTDValidate(const TixiDocumentHandle handle, const char *DTDFilename)
{

  PRINT_DEBUG2("%s %d\n", DTDFilename, handle);
  printMsg(MESSAGETYPE_ERROR, "Error: tixiDTDValidate not implemented.\n");
  assert(0);
  return SUCCESS;
}

DLL_EXPORT ReturnCode tixiGetTextElement(const TixiDocumentHandle handle, const char *elementPath, char **text)
{
  TixiDocument *document = getDocument(handle);
  xmlNodePtr element = NULL;
  ReturnCode error = SUCCESS;

  if (!document) {
    printMsg(MESSAGETYPE_ERROR, "Error: Invalid document handle.\n");
    return INVALID_HANDLE;
  }

  error = checkElement(document->xpathContext, elementPath, &element);

  if (!error) {

    xmlNodePtr children = element->children;
    char *textPtr = NULL;

    textPtr = (char *) xmlNodeListGetString(document->docPtr, children, 0);
    if ( textPtr ) {
      *text = (char *) malloc((strlen(textPtr) + 1) * sizeof(char));
      strcpy(*text, textPtr);
      xmlFree(textPtr);
    } else {
      *text = (char *) malloc(sizeof(char));
      strcpy(*text, "");
    }
    error = addToMemoryList(document, (void *) *text);
  }

  return error;
}

DLL_EXPORT ReturnCode tixiGetIntegerElement(const TixiDocumentHandle handle, const char *elementPath, int *number)
{
  char *text;
  ReturnCode error = 0;

  error = tixiGetTextElement(handle, elementPath, &text);

  if (error) {
    printMsg(MESSAGETYPE_STATUS, "Error: tixiGetTextElement returns %d in tixiGetIntegerElement.\n", error);
    return error;
  }

  *number = atoi(text);

  return SUCCESS;
}

DLL_EXPORT ReturnCode tixiGetDoubleElement(const TixiDocumentHandle handle, const char *elementPath, double *number)
{
  char *text = NULL;
  ReturnCode error = 0;

  error = tixiGetTextElement(handle, elementPath, &text);

  if (error) {
    printMsg(MESSAGETYPE_STATUS, "Error: tixiGetTextElement returns %d in tixiGetDoubleElement.\n", error);
    return error;
  }

  *number = atof(text);
  return SUCCESS;

  /* use   strtod(nptr, (char **)NULL); to check for errors */
}


DLL_EXPORT ReturnCode tixiGetBooleanElement(const TixiDocumentHandle handle, const char *elementPath, int *boolean)
{
  char *text = NULL;
  ReturnCode error = 0;

  error = tixiGetTextElement(handle, elementPath, &text);

  if (error) {
    printMsg(MESSAGETYPE_STATUS, "Error: tixiGetTextElement returns %d in tixiGetBooleanElement.\n", error);
    return error;
  }

  if( strcmp(text, "true") == 0 ) {
    *boolean = 1;
  } else if( strcmp(text, "false") == 0 ) {
    *boolean = 0;
  } else if(strcmp(text, "1") == 0) {
    *boolean = 1;
  } else if(strcmp(text, "0") == 0) {
    *boolean = 0;
  } else {
    printMsg(MESSAGETYPE_ERROR, "Error: No boolean value found at \"%s\"\n", elementPath);
    return FAILED;
  }
  return SUCCESS;
}

DLL_EXPORT ReturnCode tixiUpdateTextElement (const TixiDocumentHandle handle, const char *elementPath, const char *text)
{
  TixiDocument *document = getDocument(handle);
  xmlNodePtr element = NULL;
  xmlNodePtr newElement = NULL;
  ReturnCode error = SUCCESS;

  if (!document) {
    printMsg(MESSAGETYPE_ERROR, "Error: Invalid document handle.\n");
    return INVALID_HANDLE;
  }

  if (document->status == SAVED) {
    printMsg(MESSAGETYPE_ERROR, "Error:  Can not add element to document. Document already saved.\n");
    return ALREADY_SAVED;
  }

  error = checkElement(document->xpathContext, elementPath, &element);

  if (!error) {
    newElement = xmlNewText((xmlChar*) text);
    if(element->children) {
      xmlNodePtr nodeToReplace = element->children;
      xmlReplaceNode(nodeToReplace, newElement);
      xmlFreeNode(nodeToReplace);
    }
    else
      xmlAddChild(element, newElement);
    return SUCCESS;
  }
  return error;
}


DLL_EXPORT ReturnCode tixiUpdateDoubleElement (const TixiDocumentHandle handle, const char *elementPath, double number, const char *format)
{

  ReturnCode error = SUCCESS;
  char *textBuffer = NULL;

  if (!format) {
    format = "%g";
  };

  textBuffer = buildString(format, number);
  error = tixiUpdateTextElement(handle, elementPath, textBuffer);
  free(textBuffer);
  return error;
}


DLL_EXPORT ReturnCode tixiUpdateIntegerElement (const TixiDocumentHandle handle, const char *elementPath, int number, const char *format)
{
  ReturnCode error = SUCCESS;
  char *textBuffer = NULL;
  
  if (!format) {
    format = "%g";
  };

  textBuffer = buildString(format, number);
  error = tixiUpdateTextElement(handle, elementPath, textBuffer);
  free(textBuffer);
  return error;
}


DLL_EXPORT ReturnCode tixiUpdateBooleanElement (const TixiDocumentHandle handle, const char *elementPath, int boolean)
{
  TixiDocument *document = getDocument(handle);

  if (!document) {
    printMsg(MESSAGETYPE_ERROR, "Error: Invalid document handle.\n");
    return INVALID_HANDLE;
  }

  if (document->status == SAVED) {
    printMsg(MESSAGETYPE_ERROR, "Error:  Can not add element to document. Document already saved.\n");
    return ALREADY_SAVED;
  }

  if( boolean == 0 ) {
    return tixiUpdateTextElement (handle, elementPath, "false");
  } else if( boolean == 1 ) {
    return tixiUpdateTextElement (handle, elementPath, "true");
  } else {
    printMsg(MESSAGETYPE_ERROR, "Error: boolean is either 1 or 0 in tixiUpdateBooleanElement.\n");
    return FAILED;
  }
}


DLL_EXPORT ReturnCode tixiGetTextAttribute(const TixiDocumentHandle handle, const char *elementPath,
                                           const char *attributeName, char **text)
{
  TixiDocument *document = getDocument(handle);
  char *textPtr;
  xmlNodePtr element = NULL;
  ReturnCode error = SUCCESS;


  if (!document) {
    printMsg(MESSAGETYPE_ERROR, "Error: Invalid document handle.\n");
    return INVALID_HANDLE;
  }

  error = checkElement(document->xpathContext, elementPath, &element);
  if (!error) {
    char* prefix = NULL;
    char* name = NULL;
    ReturnCode errorCode = ATTRIBUTE_NOT_FOUND;

    extractPrefixAndName(attributeName, &prefix, &name);

    if (prefix) {
      xmlNsPtr ns = xmlSearchNs(element->doc, element, (xmlChar*) prefix);
      if (!ns) {
        printMsg(MESSAGETYPE_ERROR, "Error: unknown namespace prefix \"%s\".\n",
                 prefix);
        textPtr = NULL;
        errorCode = INVALID_NAMESPACE_PREFIX;
      }
      else {
        textPtr = (char *) xmlGetNsProp(element, (xmlChar*) name, ns->href);
      }

      free(prefix);
    }
    else {
      textPtr = (char *) xmlGetProp(element, (xmlChar *) attributeName);
    }

    free(name);

    if (textPtr) {
      *text = (char *) malloc((strlen(textPtr) + 1) * sizeof(char));
      strcpy(*text, textPtr);
      xmlFree(textPtr);
      error = addToMemoryList(document, (void *) *text);
      return error;
    }
    else {
      /*printMsg(MESSAGETYPE_ERROR,
              "Error: Attribute \"%s\" of element \"%s\" not found.  \n",
              attributeName, elementPath); */
      return errorCode;
    }
  }

  return error;
}

DLL_EXPORT ReturnCode tixiGetDoubleAttribute(const TixiDocumentHandle handle,
                                             const char *elementPath, const char *attributeName, double *number)
{
  char *text;
  ReturnCode error = 0;



  error = tixiGetTextAttribute(handle, elementPath, attributeName, &text);

  if (error) {
    printMsg(MESSAGETYPE_ERROR, "Error: tixiGetTextAttribute returns %d in tixiGetDoubleAttribute.\n", error);
    return error;
  }

  *number = atof(text);
  /* use   strtod(nptr, (char **)NULL); to check for errors */
  return SUCCESS;
}

DLL_EXPORT ReturnCode tixiGetIntegerAttribute(const TixiDocumentHandle handle,
                                              const char *elementPath, const char *attributeName, int *number)
{
  char *text;
  ReturnCode error = 0;



  error = tixiGetTextAttribute(handle, elementPath, attributeName, &text);

  if (error) {
    printMsg(MESSAGETYPE_STATUS, "Error: tixiGetTextAttribute returns %d in tixiGetIntegerAttribute.\n", error);
    return error;
  }

  *number = atoi(text);
  /* use   strtod(nptr, (char **)NULL); to check for errors */
  return SUCCESS;
}

DLL_EXPORT ReturnCode tixiGetBooleanAttribute(const TixiDocumentHandle handle,
                                              const char *elementPath, const char *attributeName, int *boolean)
{
  char *text;
  ReturnCode error = FAILED;

  if(!boolean) {
    printMsg(MESSAGETYPE_ERROR, "Error: argument boolean must not be a null pointer in tixiGetBooleanAttribute!\n");
    return FAILED;
  }

  error = tixiGetTextAttribute(handle, elementPath, attributeName, &text);

  if (error) {
    printMsg(MESSAGETYPE_STATUS, "Error: tixiGetTextAttribute returns %d in tixiGetIntegerAttribute.\n", error);
    return error;
  }

  if(strcmp(text,"true")==0 || strcmp(text,"True")==0 || strcmp(text,"1")==0){
    *boolean = 1;
  }
  else if (strcmp(text,"false")==0 || strcmp(text,"False")==0 || strcmp(text,"0")==0){
    *boolean = 0;
  }
  else {
    printMsg(MESSAGETYPE_ERROR, "Error: No boolean value found at \"%s\" for attribute \"%s\"\n", elementPath, attributeName);
    return FAILED;
  }

  return SUCCESS;
}



DLL_EXPORT ReturnCode tixiAddTextElement(const TixiDocumentHandle handle, const char *parentPath,
                                         const char *elementName, const char *text)
{
  return tixiAddTextElementAtIndex(handle, parentPath, elementName, text, -1);
}


DLL_EXPORT ReturnCode tixiAddTextElementNS(const TixiDocumentHandle handle, const char *parentPath,
                                         const char *qualifiedName, const char* namespaceURI, const char *text)
{
  return tixiAddTextElementNSAtIndex(handle, parentPath, qualifiedName, namespaceURI, text, -1);
}


ReturnCode tixiAddTextElementNSAtIndexImpl(const TixiDocumentHandle handle, const char *parentPath,
                                           const char *qualifiedName, const char* namespaceURI, const char *text, int index)
{
  TixiDocument *document = getDocument(handle);
  xmlXPathObjectPtr xpathObject = NULL;
  xmlNodeSetPtr nodes = NULL;
  xmlNodePtr parent = NULL;
  xmlNodePtr child = NULL;
  xmlNodePtr targetNode = NULL;
  char* nsPrefix = NULL;
  char* elemName = NULL;
  int i = 1;


  if (!document) {
    printMsg(MESSAGETYPE_ERROR, "Error: Invalid document handle.\n");
    return INVALID_HANDLE;
  }

  if (document->status == SAVED) {
    printMsg(MESSAGETYPE_ERROR, "Error:  Can not add element to document. Document already saved.\n");
    return ALREADY_SAVED;
  }

  if (namespaceURI) {
    extractPrefixAndName(qualifiedName, &nsPrefix, &elemName);
  }
  else {
    elemName = substring(qualifiedName, 0, -1);
  }

  if (!xmlValidateNameValue((xmlChar *) elemName)) {
    printMsg(MESSAGETYPE_ERROR, "Error: Invalid element name \"%s\"\n", elemName);
    free(elemName);
    if (nsPrefix) {
      free(nsPrefix);
    }
    return INVALID_XML_NAME;
  }

  /* Check parent element */
  xpathObject = xmlXPathEvalExpression((xmlChar *) parentPath, document->xpathContext);

  if (!xpathObject) {
    printMsg(MESSAGETYPE_ERROR, "Error: unable to evaluate xpath expression \"%s\"\n", parentPath);
    free(elemName);
    if (nsPrefix) {
      free(nsPrefix);
    }
    return INVALID_XPATH;
  }

  if (xmlXPathNodeSetIsEmpty(xpathObject->nodesetval)) {
    printMsg(MESSAGETYPE_ERROR, "Error: No element found at XPath expression \"%s\"\n", parentPath);
    xmlXPathFreeObject(xpathObject);
    free(elemName);
    if (nsPrefix) {
      free(nsPrefix);
    }
    return ELEMENT_NOT_FOUND;
  }

  nodes = xpathObject->nodesetval;

  assert(nodes);

  if (nodes->nodeNr > 1) {
    printMsg(MESSAGETYPE_ERROR,
             "Error: Element chosen by XPath \"%s\" expression is not unique. \n", parentPath);
    xmlXPathFreeObject(xpathObject);
    free(elemName);
    if (nsPrefix) {
      free(nsPrefix);
    }
    return ELEMENT_PATH_NOT_UNIQUE;
  }

  /* find node were new node should be inserted before */
  parent = nodes->nodeTab[0];
  targetNode =  parent->children;
  while(targetNode != NULL && i++ < index)
    targetNode = targetNode->next;

  child = xmlNewNode(NULL, (xmlChar *) elemName);
  if (text != NULL) {
    xmlNodePtr headingChildNode = xmlNewText( (xmlChar *) text );
    xmlAddChild( child, headingChildNode );
  }

  if (targetNode != NULL && index > 0) {
    /* insert at position index */
    xmlAddPrevSibling(targetNode, child);
  }
  else {
    /* insert at the end of the list */
    xmlAddChild(parent, child);
  }

  if (namespaceURI) {
    nodeSetNamespace(document->docPtr, child, nsPrefix, namespaceURI);
  }

  xmlXPathFreeObject(xpathObject);
  free(elemName);
  if (nsPrefix) {
    free(nsPrefix);
  }
  return SUCCESS;
}

DLL_EXPORT ReturnCode tixiAddTextElementNSAtIndex(const TixiDocumentHandle handle, const char *parentPath,
                                                  const char *qualifiedName, const char* namespaceURI, const char *text, int index)
{
  if (!namespaceURI) {
    printMsg(MESSAGETYPE_ERROR, "Error: null pointer for namespaceURI!\n");
    return INVALID_NAMESPACE_URI;
  }

  return tixiAddTextElementNSAtIndexImpl(handle, parentPath, qualifiedName, namespaceURI, text, index);
}

DLL_EXPORT ReturnCode tixiAddTextElementAtIndex(const TixiDocumentHandle handle, const char *parentPath,
                                               const char *elementName, const char *text, int index)
{
  return tixiAddTextElementNSAtIndexImpl(handle, parentPath, elementName, NULL, text, index);
}


DLL_EXPORT ReturnCode tixiAddBooleanElement(const TixiDocumentHandle handle, const char *parentPath,
                                            const char *elementName, int boolean)
{
  if( boolean == 0 ) {
    return tixiAddTextElement(handle, parentPath, elementName, "false");
  } else if( boolean == 1 ) {
    return tixiAddTextElement(handle, parentPath, elementName, "true");
  } else {
    printMsg(MESSAGETYPE_ERROR, "Error: boolean is either 1 or 0 in tixiAddBooleanElement.\n");
    return FAILED;
  }
  return SUCCESS;
}


DLL_EXPORT ReturnCode tixiAddBooleanElementNS(const TixiDocumentHandle handle, const char *parentPath,
                                            const char *qualifiedName, const char* namespaceURI, int boolean)
{
  if( boolean == 0 ) {
    return tixiAddTextElementNS(handle, parentPath, qualifiedName, namespaceURI, "false");
  } else if( boolean == 1 ) {
    return tixiAddTextElementNS(handle, parentPath, qualifiedName, namespaceURI, "true");
  } else {
    printMsg(MESSAGETYPE_ERROR, "Error: boolean is either 1 or 0 in tixiAddBooleanElementNS.\n");
    return FAILED;
  }
  return SUCCESS;
}


DLL_EXPORT ReturnCode tixiAddDoubleElement(const TixiDocumentHandle handle, const char *parentPath,
                                           const char *elementName, double number, const char *format)
{
  int error;
  char *textBuffer = NULL;


  if (!format) {
    format = "%g";
  };

  textBuffer = buildString(format, number);

  if (textBuffer) {
    error = tixiAddTextElement(handle, parentPath, elementName, textBuffer);
    free(textBuffer);
  }
  else {
    printMsg(MESSAGETYPE_ERROR, "Internal Error: Failed to allocate memory in tixiAddDoubleElement.\n");
    exit(1);
  }

  return error;
}


DLL_EXPORT ReturnCode tixiAddDoubleElementNS(const TixiDocumentHandle handle, const char *parentPath,
                                             const char *qualifiedName,const char* namespaceURI,
                                             double number, const char *format)
{
  int error;
  char *textBuffer = NULL;


  if (!format) {
    format = "%g";
  };

  textBuffer = buildString(format, number);

  if (textBuffer) {
    error = tixiAddTextElementNS(handle, parentPath, qualifiedName, namespaceURI, textBuffer);
    free(textBuffer);
  }
  else {
    printMsg(MESSAGETYPE_ERROR, "Internal Error: Failed to allocate memory in tixiAddDoubleElementNS.\n");
    exit(1);
  }

  return error;
}


DLL_EXPORT ReturnCode tixiAddIntegerElement(const TixiDocumentHandle handle, const char *parentPath,
                                            const char *elementName, int number, const char *format)
{
  int error;
  char *textBuffer = NULL;



  if (!format) {
    format = "%d";
  };

  textBuffer = buildString(format, number);

  if (textBuffer) {
    error = tixiAddTextElement(handle, parentPath, elementName, textBuffer);
    free(textBuffer);
  }
  else {
    printMsg(MESSAGETYPE_ERROR, "Internal Error: Failed to allocate memory in tixiAddIntegerElement.\n");
    exit(1);
  }

  return error;
}


DLL_EXPORT ReturnCode tixiAddIntegerElementNS(const TixiDocumentHandle handle, const char *parentPath,
                                              const char *qualifiedName, const char* namespaceURI, int number, const char *format)
{
  int error;
  char *textBuffer = NULL;



  if (!format) {
    format = "%d";
  };

  textBuffer = buildString(format, number);

  if (textBuffer) {
    error = tixiAddTextElementNS(handle, parentPath, qualifiedName, namespaceURI, textBuffer);
    free(textBuffer);
  }
  else {
    printMsg(MESSAGETYPE_ERROR, "Internal Error: Failed to allocate memory in tixiAddIntegerElementNS.\n");
    exit(1);
  }

  return error;
}


DLL_EXPORT ReturnCode tixiAddTextAttribute(const TixiDocumentHandle handle, const char *elementPath,
                                           const char *attributeName, const char *attributeValue)
{
  TixiDocument *document = getDocument(handle);

  if (!document) {
    printMsg(MESSAGETYPE_ERROR, "Error: Invalid document handle.\n");
    return INVALID_HANDLE;
  }

  if (document->status == SAVED) {
    printMsg(MESSAGETYPE_ERROR, "Error: Can not add attribute to element. Document already saved.\n");
    return ALREADY_SAVED;
  }

  return genericAddTextAttribute(document->xpathContext, elementPath, attributeName, attributeValue);
}

DLL_EXPORT ReturnCode tixiAddDoubleAttribute(const TixiDocumentHandle handle,
                                             const char *elementPath, const char *attributeName,
                                             double number, const char *format)
{
  int error;
  char *textBuffer = NULL;


  if (!format) {
    format = "%g";
  };

  textBuffer = buildString(format, number);

  if (textBuffer) {
    error = tixiAddTextAttribute(handle, elementPath, attributeName, textBuffer);
    free(textBuffer);
  }
  else {
    printMsg(MESSAGETYPE_ERROR, "Internal Error: Failed to allocate memory in tixiAddDoubleAttribute.\n");
    exit(1);
  }
  return error;
}

DLL_EXPORT ReturnCode tixiAddIntegerAttribute(const TixiDocumentHandle handle,
                                              const char *elementPath, const char *attributeName, int number, const char *format)
{
  /* Here the format string should be parsed to determine the length of the string to be generated. Instead,
     a very large char-array is allocated the string is printed into it. */
  int error;
  char *textBuffer = NULL;

  if (!format) {
    format = "%d";
  };

  textBuffer = buildString(format, number);

  if (textBuffer) {
    error = tixiAddTextAttribute(handle, elementPath, attributeName, textBuffer);
    free(textBuffer);
  }
  else {
    printMsg(MESSAGETYPE_ERROR, "Internal Error: Failed to allocate memory in tixiAddIntegerAttribute.\n");
    exit(1);
  }
  return error;
}


DLL_EXPORT ReturnCode tixiAddFloatVector (const TixiDocumentHandle handle, const char *parentPath, const char *elementName, const double *vector, const int numElements, const char* format)
{
  ReturnCode error;
  char *stringVector = NULL;
  char *textBuffer = NULL;
  int i;
  int stringSize = 0;

  if(numElements < 1) {
    return FAILED;
  }

  if (!format) {
    format = "%g";
  };

  /* calculate the size of the resulting string */
  for(i=0; i<numElements; i++) {
    textBuffer = buildString(format, vector[i]);
    stringSize += (int) strlen(textBuffer);
    free(textBuffer);
  }

  /* allocate memory */
  stringVector = (char *) malloc(sizeof(char) * (stringSize + numElements + 1));

  /* copy strings to stringVector */
  stringVector[0] = '\0';
  textBuffer = buildString(format, vector[0]);
  strcat(stringVector, textBuffer);
  free(textBuffer);
  for(i=1; i<numElements; i++) {
    textBuffer = buildString(format, vector[i]);
    strcat(stringVector, VECTOR_SEPARATOR);
    strcat(stringVector, textBuffer);
    free(textBuffer);
  }

  /* Add element */
  error = tixiAddTextElement(handle, parentPath, elementName, stringVector);
  if(error != SUCCESS) {
    return error;
  }

  /* add attribute */
  textBuffer = (char *) malloc(sizeof(char) * (strlen(parentPath) + strlen(elementName) + 2));
  textBuffer[0] = '\0';
  strcat(textBuffer, parentPath);
  strcat(textBuffer, "/");
  strcat(textBuffer, elementName);
  error = tixiAddTextAttribute(handle, textBuffer, MAPTYPE_IDENTIFIER, VECTOR_ATTRIBUTE_NAME);
  if(error != SUCCESS) {
    return error;
  }
  free(textBuffer);
  free(stringVector);

  return SUCCESS;
}

DLL_EXPORT ReturnCode tixiUpdateFloatVector (const TixiDocumentHandle handle, const char *path, const double *vector, const int numElements, const char* format)
{
  ReturnCode error;
  char *stringVector = NULL;
  char *textBuffer = NULL;
  int i;
  int stringSize = 0;

  if(numElements < 1) {
    return FAILED;
  }

  if (!format) {
    format = "%g";
  };

  /* calculate the size of the resulting string */
  for(i=0; i<numElements; i++) {
    textBuffer = buildString(format, vector[i]);
    stringSize += (int) strlen(textBuffer);
    free(textBuffer);
  }

  /* allocate memory */
  stringVector = (char *) malloc(sizeof(char) * (stringSize + numElements + 1));

  /* copy strings to stringVector */
  stringVector[0] = '\0';
  textBuffer = buildString(format, vector[0]);
  strcat(stringVector, textBuffer);
  free(textBuffer);
  for(i=1; i<numElements; i++) {
    textBuffer = buildString(format, vector[i]);
    strcat(stringVector, VECTOR_SEPARATOR);
    strcat(stringVector, textBuffer);
    free(textBuffer);
  }

  error = tixiUpdateTextElement(handle, path, stringVector);
  free(stringVector);

  return error;
}



DLL_EXPORT ReturnCode tixiRemoveAttribute(const TixiDocumentHandle handle, const char *elementPath, const char *attributeName)
{
  xmlNodePtr parent;
  int retVal = 0;
  ReturnCode errorCode = SUCCESS;
  char* prefix = NULL;
  char* name = NULL;

  retVal = getNodePtrFromElementPath(handle, elementPath, &parent);

  if (retVal != SUCCESS) {
    return retVal;
  }

  if(parent == NULL) {
    return INVALID_XPATH;
  }

  extractPrefixAndName(attributeName, &prefix, &name);
  if (!prefix) {
    retVal = xmlUnsetProp(parent, (xmlChar *) attributeName);
  }
  else {
    xmlNsPtr ns = xmlSearchNs(parent->doc, parent, (xmlChar *) prefix);
    if (!ns) {
      printMsg(MESSAGETYPE_ERROR, "Error: unknown namespace prefix \"%s\".\n",
               prefix);
      errorCode = INVALID_NAMESPACE_PREFIX;
    }
    else {
      retVal = xmlUnsetNsProp(parent, ns, (xmlChar*) name);
    }
    free(prefix);
  }
  free(name);

  if (errorCode != SUCCESS) {
      return errorCode;
  }

  if (retVal != 0) {
      return ATTRIBUTE_NOT_FOUND;
  }
  else {
      return SUCCESS;
  }
}


DLL_EXPORT ReturnCode tixiRemoveElement(const TixiDocumentHandle handle, const char *elementPath)
{
  xmlNodePtr parent = NULL;
  ReturnCode retVal;

  retVal = getNodePtrFromElementPath(handle, elementPath, &parent);

  if(parent != NULL) {
    xmlUnlinkNode(parent);
    xmlFreeNode(parent);
    return SUCCESS;
  }
  else {
    return retVal;
  }
}


DLL_EXPORT ReturnCode tixiGetNamedChildrenCount(const TixiDocumentHandle handle,
                                                const char *elementPath, const char *childName, int *count)
{
  TixiDocument *document = getDocument(handle);
  xmlXPathObjectPtr xpathObject = NULL;
  xmlNodeSetPtr nodes = NULL;
  char * prefix = NULL;
  char * childTagName = NULL;

  int iNode;
  char *childElementPath =
      (char *) malloc(sizeof(char) * (strlen(elementPath) + strlen(childName) + 2));
  char *allChildren = (char *) malloc(sizeof(char) * (strlen(elementPath) + 3));



  if (!document) {
    printMsg(MESSAGETYPE_ERROR, "Error: Invalid document handle.\n");
    free(childElementPath);
    free(allChildren);
    return INVALID_HANDLE;
  }

  childElementPath[0] = '\0';
  allChildren[0] = '\0';

  strcat(childElementPath, elementPath);
  strcat(childElementPath, "/");
  strcat(childElementPath, childName);

  strcat(allChildren, elementPath);
  strcat(allChildren, "/*");

  *count = 0;

  /* first check parent */
  xpathObject = xmlXPathEvalExpression((xmlChar *) elementPath, document->xpathContext);

  if (!xpathObject) {
    printMsg(MESSAGETYPE_ERROR, "Error: unable to evaluate xpath expression \"%s\"\n", elementPath);
    free(childElementPath);
    free(allChildren);
    return INVALID_XPATH;
  }

  if (xmlXPathNodeSetIsEmpty(xpathObject->nodesetval)) {
    xmlXPathFreeObject(xpathObject);
    free(childElementPath);
    free(allChildren);
    return ELEMENT_NOT_FOUND;
  }

  nodes = xpathObject->nodesetval;
  assert(nodes);

  if (nodes->nodeNr > 1) {
    printMsg(MESSAGETYPE_ERROR,
             "Error: Element chosen by XPath \"%s\" expression is not unique. \n", elementPath);
    free(childElementPath);
    free(allChildren);
    xmlXPathFreeObject(xpathObject);
    return ELEMENT_PATH_NOT_UNIQUE;
  }

  xmlXPathFreeObject(xpathObject);

  /* check if there are children at all */


  xpathObject = xmlXPathEvalExpression((xmlChar *) allChildren, document->xpathContext);

  if (!xpathObject) {
    printMsg(MESSAGETYPE_ERROR, "Error: unable to evaluate xpath expression \"%s\"\n", allChildren);
    free(childElementPath);
    free(allChildren);
    return INVALID_XPATH;
  }


  if (xmlXPathNodeSetIsEmpty(xpathObject->nodesetval)) {

    /* parent has no child at all, return child count 0 */
    *count = 0;
    xmlXPathFreeObject(xpathObject);
    free(childElementPath);
    free(allChildren);
    return SUCCESS;
  }

  xmlXPathFreeObject(xpathObject);

  /* now check child */

  xpathObject = xmlXPathEvalExpression((xmlChar *) childElementPath, document->xpathContext);

  if (!xpathObject) {
    printMsg(MESSAGETYPE_ERROR, "Error: unable to evaluate xpath expression \"%s\"\n", childElementPath);
    free(childElementPath);
    free(allChildren);
    return INVALID_XPATH;
  }

  if (xmlXPathNodeSetIsEmpty(xpathObject->nodesetval)) {

    /* parent has no child with name childName , return child count 0 */
    *count = 0;
    xmlXPathFreeObject(xpathObject);
    free(childElementPath);
    free(allChildren);
    return SUCCESS;
  }

  nodes = xpathObject->nodesetval;
  assert(nodes);

  extractPrefixAndName(childName, &prefix, &childTagName);

  if (prefix) {
    free(prefix);
  }

  for (iNode = 0; iNode < nodes->nodeNr; iNode++) {
    xmlNodePtr child = nodes->nodeTab[iNode];
    assert(child);

    if (child->type == XML_ELEMENT_NODE) {

      if (!strcmp(childTagName, (char *) child->name)) {
        (*count)++;
      }
    }
  }

  xmlXPathFreeObject(xpathObject);
  free(childElementPath);
  free(allChildren);
  free(childTagName);
  return SUCCESS;
}

DLL_EXPORT ReturnCode tixiAddDoubleListWithAttributes(const TixiDocumentHandle handle,
                                                      const char *parentPath, const char *listName,
                                                      const char *childName,
                                                      const char *attributeName,
                                                      const double *values, const char *format,
                                                      const char **attributes, int nValues)
{
  ReturnCode error = -1;
  int iValue;
  TixiDocument *document = getDocument(handle);
  xmlNodePtr parent = NULL;



  if (!document) {
    printMsg(MESSAGETYPE_ERROR, "Error: Invalid document handle.\n");
    return INVALID_HANDLE;
  }

  if (document->status == SAVED) {
    printMsg(MESSAGETYPE_ERROR, "Error:  Can not add element to document. Document already saved.\n");
    return ALREADY_SAVED;
  }

  if (!xmlValidateNameValue((xmlChar *) listName)) {
    printMsg(MESSAGETYPE_ERROR, "Error: Invalid element name \"%s\"\n", listName);
    return INVALID_XML_NAME;
  }

  if (!xmlValidateNameValue((xmlChar *) childName)) {
    printMsg(MESSAGETYPE_ERROR, "Error: Invalid element name \"%s\"\n", childName);
    return INVALID_XML_NAME;
  }

  if (!xmlValidateNameValue((xmlChar *) attributeName)) {
    printMsg(MESSAGETYPE_ERROR, "Error: Invalid element name \"%s\"\n", attributeName);
    return INVALID_XML_NAME;
  }

  if (!format) {
    format = "%g";
  }

  error = checkElement(document->xpathContext, parentPath, &parent);
  if (!error) {

    /* create node containing the list */
    xmlNodePtr listNode = xmlNewNode(nameSpace, (xmlChar *) listName);

    for (iValue = 0; iValue < nValues; iValue++) {

      xmlNodePtr child;
      char *textBuffer = buildString(format, values[iValue]);

      if (!textBuffer) {
        printMsg(MESSAGETYPE_ERROR,
                 "Internal Error: Failed to allocate memory in tixiAddDoubleListWithAttributes.\n");
        exit(1);
      }

      child = xmlNewTextChild(listNode, nameSpace, (xmlChar *) childName, (xmlChar *) textBuffer);

      free(textBuffer);
      textBuffer = NULL;

      if (!child) {
        printMsg(MESSAGETYPE_ERROR,
                 "Error:  Failed to add child #%d to list  element \"%s\".\n", iValue + 1, listName);
        xmlFreeNode(listNode);
        return FAILED;
      }

      if (!xmlNewProp(child, (xmlChar *) attributeName, (xmlChar *) attributes[iValue])) {
        printMsg(MESSAGETYPE_ERROR,
                 "Error:  Failed to add attribute \"%s\" to element \"%s/%-s%-s\".\n",
                 attributeName, parentPath, listName, childName);
        xmlFreeNode(listNode);
        return FAILED;
      }
    }


    if (!xmlAddChild(parent, listNode)) {
      printMsg(MESSAGETYPE_ERROR,
               "Error:  Failed to add list element \"%s\" to parent \"%s\".\n",
               listName, parentPath);
      xmlFreeNode(listNode);
      return FAILED;
    }
    else {
      return SUCCESS;
    }
  }
  else {
    return error;
  }
}

DLL_EXPORT ReturnCode tixiAddPoint(const TixiDocumentHandle handle, const char *parentPath,
                                   double x, double y, double z, const char *format)
{
  ReturnCode error = -1;
  TixiDocument *document = getDocument(handle);
  xmlNodePtr parent = NULL;

  if (!document) {
    printMsg(MESSAGETYPE_ERROR, "Error: Invalid document handle.\n");
    return INVALID_HANDLE;
  }

  if (document->status == SAVED) {
    printMsg(MESSAGETYPE_ERROR, "Error:  Can not add element to document. Document already saved.\n");
    return ALREADY_SAVED;
  }

  if (!format) {
    format = "%g";
  }

  error = checkElement(document->xpathContext, parentPath, &parent);

  if (!error) {

    /* create node containing the point coordinates */

    xmlNodePtr pointNode = parent;
    xmlNodePtr child = NULL;

    /* add x coordinate */
    char *textBuffer = buildString(format, x);

    if (textBuffer) {

      child = xmlNewTextChild(pointNode, nameSpace, (xmlChar *) "x", (xmlChar *) textBuffer);
      free(textBuffer);
      textBuffer = NULL;

      if (!child) {
        printMsg(MESSAGETYPE_ERROR, "Error:  Failed to add %1s coordinate to point element.\n", "x");
        xmlFreeNode(pointNode);
        return FAILED;
      }
    }
    else {
      printMsg(MESSAGETYPE_ERROR, "Internal Error: Failed to allocate memory in tixiAddPoint.\n");
      exit(1);
    }

    /* add y coordinate */
    textBuffer = buildString(format, y);

    if (textBuffer) {

      child = xmlNewTextChild(pointNode, nameSpace, (xmlChar *) "y", (xmlChar *) textBuffer);
      free(textBuffer);
      textBuffer = NULL;

      if (!child) {
        printMsg(MESSAGETYPE_ERROR, "Error:  Failed to add %1s coordinate to point element.\n", "y");
        xmlFreeNode(pointNode);
        return FAILED;
      }
    }
    else {
      printMsg(MESSAGETYPE_ERROR, "Internal Error: Failed to allocate memory in tixiAddPoint.\n");
      exit(1);
    }


    /* add z coorinate */
    textBuffer = buildString(format, z);

    if (textBuffer) {

      child = xmlNewTextChild(pointNode, nameSpace, (xmlChar *) "z", (xmlChar *) textBuffer);
      free(textBuffer);
      textBuffer = NULL;

      if (!child) {
        printMsg(MESSAGETYPE_ERROR, "Error:  Failed to add %1s coordinate to point element.\n", "z");
        xmlFreeNode(pointNode);
        return FAILED;
      }
    }
    else {
      printMsg(MESSAGETYPE_ERROR, "Internal Error: Failed to allocate memory in tixiAddPoint.\n");
      exit(1);
    }
  }
  else {
    return error;
  }
  return SUCCESS;
}


DLL_EXPORT ReturnCode tixiGetPoint(const TixiDocumentHandle handle, const char *parentPath,
                                   double *x, double *y, double *z)
{
  int ignoreMissingElement = 1;
  int error;



  error = getPoint(handle, parentPath, 1, ignoreMissingElement, x, y, z);

  return error;
}

DLL_EXPORT ReturnCode tixiAddExternalLink(const TixiDocumentHandle handle, const char *parentPath,
                                          const char *url, const char *fileFormat)
{
  ReturnCode error = tixiAddTextElement(handle, parentPath, "externalFile", url);
  char *elementNameWithLeadingSlash = "/externalFile";
  size_t bufferLength = strlen(parentPath) + strlen(elementNameWithLeadingSlash) + 1;
  char *textBuffer = (char *) malloc(sizeof(char) * bufferLength);



  if (!textBuffer) {
    printMsg(MESSAGETYPE_ERROR, "Internal Error: Failed to allocate memory in tixiAddExternalLink.\n");
    exit(1);
  }

  strcpy(textBuffer, parentPath);
  strcat(textBuffer, elementNameWithLeadingSlash);

  if (!error) {
    if (fileFormat) {
      error = tixiAddTextAttribute(handle, textBuffer, "format", fileFormat);
    }
  }
  free(textBuffer);
  textBuffer = NULL;

  return error;
}


DLL_EXPORT ReturnCode tixiUsePrettyPrint(TixiDocumentHandle handle, int usePrettyPrint)
{
  TixiDocument *document = getDocument(handle);

  if (!document) {
    printMsg(MESSAGETYPE_ERROR, "Error: Invalid document handle.\n");
    return INVALID_HANDLE;
  }

  if ((usePrettyPrint>1)||(usePrettyPrint < 0)) {
    return FAILED;
  }

  document->usePrettyPrint = usePrettyPrint;
  return SUCCESS;
}

DLL_EXPORT ReturnCode tixiSetPrintMsgFunc(TixiPrintMsgFnc func)
{
  tixiInit();
  if (func) {
    tixiMessageHandler = func;
    return SUCCESS;
  }
  else {
    return FAILED;
  }
}

DLL_EXPORT TixiPrintMsgFnc tixiGetPrintMsgFunc()
{
    tixiInit();

    return tixiMessageHandler;
}


DLL_EXPORT ReturnCode tixiGetVectorSize (const TixiDocumentHandle handle, const char *vectorPath, int *nElements)
{
  ReturnCode error = FAILED;
  char *tmpString = NULL;
  char *token = NULL;
  *nElements = 0;

  error = tixiGetTextAttribute(handle, vectorPath, MAPTYPE_IDENTIFIER, &tmpString);
  if (error != SUCCESS) {
    return error;
  } else {
    tmpString = stringToLower(tmpString);   /* side effect: changes original string anyway */
    if ((strcmp(tmpString, VECTOR_ATTRIBUTE_NAME))) {   /* if not of type "vector": */
      return ELEMENT_NOT_FOUND;
    }
  }

  error = tixiGetTextElement(handle, vectorPath, &tmpString);
  if(error != SUCCESS) {
    return error;
  }

  token = strtok(tmpString, VECTOR_SEPARATOR);
  while(token != NULL) {
    *nElements = *nElements + 1;
    token = strtok(0, VECTOR_SEPARATOR);
  }
  return SUCCESS;
}

DLL_EXPORT ReturnCode tixiGetArrayDimensions (const TixiDocumentHandle handle,
                                              const char *arrayPath, int *dimensions)
{

  return tixiGetArrayElementCount(handle, arrayPath, "vector", dimensions);
}


DLL_EXPORT ReturnCode tixiGetArrayDimensionNames (const TixiDocumentHandle handle,
                                                  const char *arrayPath,
                                                  char **dimensionNames)
{

  return tixiGetArrayElementNames(handle, arrayPath, "vector", dimensionNames);
}


DLL_EXPORT ReturnCode tixiGetArrayDimensionSizes (const TixiDocumentHandle handle, const char *arrayPath,
                                                  int *sizes, int *arraySizes)
{
  TixiDocument *document = getDocument(handle);
  xmlXPathObjectPtr xpathObject = NULL;
  xmlNodeSetPtr nodes = NULL;
  xmlNodePtr node = NULL;
  char *suffix = "/*[@mapType=\"vector\"]";    /* find all vectors tag in subelements of the given path */
  char *xpathSubElementsName = (char *) malloc((strlen(arrayPath) + strlen(suffix) + 1) * sizeof(char));
  char *tmpContent = NULL;
  char *tmpContCpy = NULL;
  char *token = NULL;
  int dimensions = 0;
  int dim = 0;



  if (!document) {
    printMsg(MESSAGETYPE_ERROR, "Error: Invalid document handle.\n");
    free(xpathSubElementsName);
    return INVALID_HANDLE;
  }

  xpathSubElementsName[0] = '\0';
  strcpy(xpathSubElementsName, arrayPath);
  strcat(xpathSubElementsName, suffix);    /* meaning: find all vector sub elements */

  xpathObject = xmlXPathEvalExpression((xmlChar *) xpathSubElementsName, document->xpathContext);

  if (!xpathObject) {
    printMsg(MESSAGETYPE_ERROR, "Error: unable to evaluate xpath expression \"%s\"\n", xpathSubElementsName);
    free(xpathSubElementsName);
    return INVALID_XPATH;
  }

  if (xmlXPathNodeSetIsEmpty(xpathObject->nodesetval)) {
    printMsg(MESSAGETYPE_ERROR, "Error: No element found at XPath expression \"%s\"\n", xpathSubElementsName);
    free(xpathSubElementsName);
    xmlXPathFreeObject(xpathObject);
    return ELEMENT_NOT_FOUND;
  }

  nodes = xpathObject->nodesetval;
  assert(nodes);

  dimensions = nodes->nodeNr;    /* number of sub elements conforming to the constructed XPath */
  assert(dimensions >= 0);
  *arraySizes = 1;    /* prepare product calculation */
  for (dim = 0; dim < dimensions; dim ++) {
    node = nodes->nodeTab[dim];
    assert(node);
    if (!(node->children)) {
      printMsg(MESSAGETYPE_ERROR, "Error: No string content in vector subelement at XPath expression \"%s\"\n", xpathSubElementsName);
      free(xpathSubElementsName);
      xmlXPathFreeObject(xpathObject);
      return ELEMENT_NOT_FOUND;
    }
    node = node->children;    /* get children, because there must be a node containing the string value */
    assert(node);
    tmpContent = (char *) xmlNodeListGetString(document->docPtr, node, 0);
    if (tmpContent) {
      tmpContCpy = (char *) malloc((strlen(tmpContent) + 1) * sizeof(char));
      tmpContCpy[0] = '\0';
      strcpy(tmpContCpy, tmpContent);
    } else {
      tmpContCpy = (char *) malloc(sizeof(char));
      tmpContCpy[0] = '\0';
    }
    xmlFree(tmpContent);
    /* tokenize string into distinct elements to count */
    sizes[dim] = 0;    /* reset before increasing for each token */
    token = strtok(tmpContCpy, VECTOR_SEPARATOR);
    while (token != NULL) {
      sizes[dim] ++;  /* count no. of values in dimension */
      token = strtok(0, VECTOR_SEPARATOR);
    }
    free(tmpContCpy);
    *arraySizes *= sizes[dim];    /* calculate product */
  }
  free(xpathSubElementsName);
  xmlXPathFreeObject(xpathObject);
  return SUCCESS;
}


DLL_EXPORT ReturnCode tixiGetArrayDimensionValues (const TixiDocumentHandle handle, const char *arrayPath,
                                                   const int dimension, double *dimensionValues)
{
  TixiDocument *document = getDocument(handle);
  xmlXPathObjectPtr xpathObject = NULL;
  xmlNodeSetPtr nodes = NULL;
  xmlNodePtr node = NULL;
  const char *suffix = "/*[@mapType=\"vector\"]";
  char *xpathSubElementsName = (char *) malloc((strlen(arrayPath) + strlen(suffix)) * sizeof(char) + 4);
  char *tmpContent = NULL;    /* reference copy */
  char *tmpContCpy = NULL;    /* tokenizer */
  char *token = NULL;
  int dimensions = 0;
  int count = 0;



  if (!document) {
    printMsg(MESSAGETYPE_ERROR, "Error: Invalid document handle.\n");
    free(xpathSubElementsName);
    return INVALID_HANDLE;
  }

  xpathSubElementsName[0] = '\0';
  strcpy(xpathSubElementsName, arrayPath);
  strcat(xpathSubElementsName, suffix);    /* meaning: find all vector sub elements */

  xpathObject = xmlXPathEvalExpression((xmlChar *) xpathSubElementsName, document->xpathContext);

  if (!xpathObject) {
    printMsg(MESSAGETYPE_ERROR, "Error: unable to evaluate xpath expression \"%s\"\n", xpathSubElementsName);
    free(xpathSubElementsName);
    return INVALID_XPATH;
  }

  if (xmlXPathNodeSetIsEmpty(xpathObject->nodesetval)) {
    printMsg(MESSAGETYPE_ERROR, "Error: No element found at XPath expression \"%s\"\n", xpathSubElementsName);
    free(xpathSubElementsName);
    xmlXPathFreeObject(xpathObject);
    return ELEMENT_NOT_FOUND;
  }

  nodes = xpathObject->nodesetval;
  assert(nodes);

  dimensions = nodes->nodeNr;    /* number of sub elements conforming to the constructed XPath */

  if (dimensions <= dimension) {    /* check if there are enough elements to read from */
    printMsg(MESSAGETYPE_ERROR, "Error: Not enough dimensions found for array \"%s\"\n", xpathSubElementsName);
    free(xpathSubElementsName);
    xmlXPathFreeObject(xpathObject);
    return ELEMENT_NOT_FOUND;
  }
  assert(dimensions >= 0);

  node = nodes->nodeTab[dimension];
  assert(node);
  node = node->children;    /* get children, because there must be a node containing the string value */
  assert(node);
  tmpContent = (char *) xmlNodeListGetString(document->docPtr, node, 0);
  if (tmpContent) {
    tmpContCpy = (char *) malloc((strlen(tmpContent) + 1) * sizeof(char));
    tmpContCpy[0] = '\0';
    strcpy(tmpContCpy, tmpContent);
  } else {
    tmpContCpy = (char *) malloc(sizeof(char));
    tmpContCpy[0] = '\0';
  }
  /* tokenize string into distinct elements to count */
  token = strtok(tmpContCpy, VECTOR_SEPARATOR);
  while (token != NULL) {
    dimensionValues[count ++]= atof(token);
    token = strtok(0, VECTOR_SEPARATOR);
  }
  free(tmpContCpy);
  xmlFree(tmpContent);

  /* clean up */
  free(xpathSubElementsName);
  xmlXPathFreeObject(xpathObject);
  return SUCCESS;
}


DLL_EXPORT ReturnCode tixiGetArrayParameters (const TixiDocumentHandle handle,
                                              const char *arrayPath, int *parameters)
{

  return tixiGetArrayElementCount(handle, arrayPath, "array", parameters);
}


DLL_EXPORT ReturnCode tixiGetArrayParameterNames (const TixiDocumentHandle handle,
                                                  const char *arrayPath,
                                                  char **parameterNames)
{

  return tixiGetArrayElementNames(handle, arrayPath, "array", parameterNames);
}


DLL_EXPORT ReturnCode tixiGetArray (const TixiDocumentHandle handle, const char *arrayPath,
                                    const char *elementName, int arraySize, double **pValues)
{
  TixiDocument *document = getDocument(handle);
  xmlXPathObjectPtr xpathObject = NULL;
  xmlNodeSetPtr nodes = NULL;
  xmlNodePtr node = NULL;
  char *xpathSubElementsName = (char *) malloc((strlen(arrayPath) + strlen("/") + strlen(elementName)) * sizeof(char) + 1);
  char *attributeName = NULL;
  char *tmpContent = NULL;    /* reference copy */
  char *tmpContCpy = NULL;    /* tokenizer */
  char *token = NULL;
  double * tmpArray = NULL;
  int count = 0;


  if (!document) {
    printMsg(MESSAGETYPE_ERROR, "Error: Invalid document handle.\n");
    free(xpathSubElementsName);
    return INVALID_HANDLE;
  }

  /* build xpath to the subelement requested */
  xpathSubElementsName[0] = '\0';
  strcpy(xpathSubElementsName, arrayPath);
  strcat(xpathSubElementsName, "/");
  strcat(xpathSubElementsName, elementName);

  xpathObject = xmlXPathEvalExpression((xmlChar *) xpathSubElementsName, document->xpathContext);

  if (!xpathObject) {
    printMsg(MESSAGETYPE_ERROR, "Error: unable to evaluate xpath expression \"%s\"\n", xpathSubElementsName);
    free(xpathSubElementsName);
    return INVALID_XPATH;
  }

  if (xmlXPathNodeSetIsEmpty(xpathObject->nodesetval)) {
    printMsg(MESSAGETYPE_ERROR, "Error: No element found at XPath expression \"%s\"\n", xpathSubElementsName);
    free(xpathSubElementsName);
    xmlXPathFreeObject(xpathObject);
    return ELEMENT_NOT_FOUND;
  }

  nodes = xpathObject->nodesetval;
  assert(nodes);

  node = nodes->nodeTab[0];
  assert(node);

  /* check if the element has the correct attribute and value */
  attributeName = (char *) xmlGetProp(node, (xmlChar *) "mapType");

  if (attributeName) {
    if (strcmp(attributeName, "array") != 0) {
      printMsg(MESSAGETYPE_ERROR,
               "Error: The given sub element is not of mapType= \"array\" in \"%s\": %s \n",
               xpathSubElementsName,
               attributeName);
      free(xpathSubElementsName);
      xmlFree(attributeName);
      xmlXPathFreeObject(xpathObject);
      return ATTRIBUTE_NOT_FOUND;
    }
  } else {
    printMsg(MESSAGETYPE_ERROR,
             "Error: The given sub element has no attribute mapType in \"%s\". \n",
             xpathSubElementsName);
    free(xpathSubElementsName);
    xmlXPathFreeObject(xpathObject);
    return ATTRIBUTE_NOT_FOUND;
  }

  node = node->children;    /* get children, because there must be a node containing the string value */
  if(!(node)) {
    printMsg(MESSAGETYPE_ERROR,
             "Error: The given sub element has no children in \"%s\". \n",
             xpathSubElementsName);
    free(xpathSubElementsName);
    xmlFree(attributeName);
    xmlXPathFreeObject(xpathObject);
    return ATTRIBUTE_NOT_FOUND;
  }
  assert(node);
  tmpContent = (char *) xmlNodeListGetString(document->docPtr, node, 0);
  if (tmpContent) {
    tmpContCpy = (char *) malloc((strlen(tmpContent) + 1) * sizeof(char));
    tmpContCpy[0] = '\0';
    strcpy(tmpContCpy, tmpContent);
    xmlFree(tmpContent);
  } else {
    tmpContCpy = (char *) malloc(sizeof(char));
    tmpContCpy[0] = '\0';
  }

  /* allocate memory for array */
  tmpArray = (double *) malloc(sizeof(double) * arraySize);

  /* tokenize string into distinct elements to separate values */
  token = strtok(tmpContCpy, VECTOR_SEPARATOR);
  while (token != NULL && count < arraySize) {
    tmpArray[count ++] = atof(token);
    token = strtok(0, VECTOR_SEPARATOR);
  }
  free(tmpContCpy);

  /* clean up */
  free(xpathSubElementsName);
  xmlFree(attributeName);
  xmlXPathFreeObject(xpathObject);

  //check if number of entries is the same as arraySize
  if(count != arraySize || token != NULL){
    printMsg(MESSAGETYPE_ERROR,
             "Error: the number of elements of array \"%s\" does not match the specified size of %d (should be %d)!\n",
             elementName, arraySize, count);
    free(tmpArray);
    return NON_MATCHING_SIZE;
  }

  addToMemoryList(document, tmpArray);
  *pValues = tmpArray;
  return SUCCESS;
}


DLL_EXPORT double tixiGetArrayValue(const double *array, const int *dimSize, const int *dimPos, const int dims)
{
  int i = 0;
  int index = 0;

  assert(dims > 0);

  /*
       calculate position in array according to
       index = k + j*nk * i*nk*nj
             = k + nk*(j + nj*i)
    */
  for(i = 0; i < dims-1; ++i){
    index += dimPos [i];
    index *= dimSize[i+1];
  }
  index += dimPos[dims-1];

  return array[index];
}


DLL_EXPORT ReturnCode tixiGetArrayElementCount (const TixiDocumentHandle handle, const char *arrayPath,
                                                const char *elementType, int *elements)
{
  TixiDocument *document = getDocument(handle);
  xmlXPathObjectPtr xpathObject = NULL;
  xmlNodeSetPtr nodes = NULL;
  char *infix = "/*[@mapType=\"";
  char *xpathSubElementsName = NULL;

  if (!document || !document->docPtr) {
    printMsg(MESSAGETYPE_ERROR, "Error: Invalid document handle.\n");
    return INVALID_HANDLE;
  }

  xpathSubElementsName = (char *) malloc((strlen(arrayPath) + strlen(infix) + strlen(elementType) + 2 + 1 + 4) * sizeof(char));

  xpathSubElementsName[0] = '\0';
  strcpy(xpathSubElementsName, arrayPath);
  strcat(xpathSubElementsName, infix);
  strcat(xpathSubElementsName, elementType);
  strcat(xpathSubElementsName, "\"]");

  xpathObject = xmlXPathEvalExpression((xmlChar *) xpathSubElementsName, document->xpathContext);

  if (!xpathObject) {
    printMsg(MESSAGETYPE_ERROR, "Error: unable to evaluate xpath expression \"%s\"\n", xpathSubElementsName);
    free(xpathSubElementsName);
    return INVALID_XPATH;
  }

  if (xmlXPathNodeSetIsEmpty(xpathObject->nodesetval)) {
    printMsg(MESSAGETYPE_ERROR, "Error: No element found at XPath expression \"%s\"\n", xpathSubElementsName);
    free(xpathSubElementsName);
    xmlXPathFreeObject(xpathObject);
    return ELEMENT_NOT_FOUND;
  }

  nodes = xpathObject->nodesetval;
  assert(nodes);

  *elements = nodes->nodeNr;    /* number of sub elements conforming to the constructed XPath */
  free(xpathSubElementsName);
  xmlXPathFreeObject(xpathObject);
  return SUCCESS;
}


DLL_EXPORT ReturnCode tixiGetArrayElementNames (const TixiDocumentHandle handle, const char *arrayPath,
                                                const char * elementType, char **elementNames)
{
  TixiDocument *document = getDocument(handle);
  xmlXPathObjectPtr xpathObject = NULL;
  xmlNodeSetPtr nodes = NULL;
  xmlNodePtr node = NULL;
  char *infix = "/*[@mapType=\"";    /* find all arrays in subelements of the given path */
  int elements,elem = 0;
  char *xpathSubElementsName = NULL;

  if (!document) {
    printMsg(MESSAGETYPE_ERROR, "Error: Invalid document handle.\n");
    return INVALID_HANDLE;
  }

  xpathSubElementsName = (char *) malloc((strlen(arrayPath) + strlen(infix) + strlen(elementType) + 2 + 1) * sizeof(char));
  xpathSubElementsName[0] = '\0';
  strcpy(xpathSubElementsName, arrayPath);
  strcat(xpathSubElementsName, infix);
  strcat(xpathSubElementsName, elementType);
  strcat(xpathSubElementsName, "\"]");

  xpathObject = xmlXPathEvalExpression((xmlChar *) xpathSubElementsName, document->xpathContext);

  if (!xpathObject) {
    printMsg(MESSAGETYPE_ERROR, "Error: unable to evaluate xpath expression \"%s\"\n", xpathSubElementsName);
    free(xpathSubElementsName);
    return INVALID_XPATH;
  }

  if (xmlXPathNodeSetIsEmpty(xpathObject->nodesetval)) {
    printMsg(MESSAGETYPE_ERROR, "Error: No element found at XPath expression \"%s\"\n", xpathSubElementsName);
    free(xpathSubElementsName);
    xmlXPathFreeObject(xpathObject);
    return ELEMENT_NOT_FOUND;
  }

  nodes = xpathObject->nodesetval;
  assert(nodes);

  elements = nodes->nodeNr;    /* number of sub elements conforming to the constructed XPath */
  assert(elements >= 0);
  for (elem = 0; elem < elements; elem ++) {
    node = nodes->nodeTab[elem];
    assert(node);
    /* now allocate the string */
    elementNames[elem] = (char *) malloc((strlen((char*) node->name) + 1) * sizeof(char));
    strcpy(elementNames[elem], (char*)node->name);    /* copy tag name */
  }
  free(xpathSubElementsName);
  xmlXPathFreeObject(xpathObject);
  return SUCCESS;
}


DLL_EXPORT ReturnCode tixiGetFloatVector (const TixiDocumentHandle handle, const char *vectorPath,
                                          double **vectorArray, const int eNumber)
{
  ReturnCode error;
  char *tmpString = NULL;
  int count = 0;
  char *token = NULL;
  TixiDocument *document = NULL;


  error = tixiGetTextAttribute(handle, vectorPath, MAPTYPE_IDENTIFIER, &tmpString);
  if (error != SUCCESS) {
    return error;
  } else {
    tmpString = stringToLower(tmpString);
    if ((strcmp(tmpString, VECTOR_ATTRIBUTE_NAME))) {
      return ELEMENT_NOT_FOUND;
    }
  }

  error = tixiGetTextElement(handle, vectorPath, &tmpString); /* check if element is of right type */
  if (error != SUCCESS) {
    return error;
  }

  *vectorArray = (double *) malloc(eNumber * sizeof(double));
  document = getDocument(handle);
  addToMemoryList(document, *vectorArray);

  token = strtok(tmpString, VECTOR_SEPARATOR);    /* modifies tmpString */
  while(token != NULL) {  /* as long as there are tokens */
    if (count == eNumber) {
      return SUCCESS;
    }
    (*vectorArray)[count ++] = atof(token);    /* starting with zero */
    token = strtok(0, VECTOR_SEPARATOR);
  }
  return count < eNumber ? INDEX_OUT_OF_RANGE : SUCCESS;
}


DLL_EXPORT ReturnCode tixiCreateElement (const TixiDocumentHandle handle, const char *parentPath, const char *elementName)
{
  char *text = NULL;
  return tixiAddTextElement(handle, parentPath, elementName, text);
}


DLL_EXPORT ReturnCode tixiCreateElementNS (const TixiDocumentHandle handle, const char *parentPath, const char *qualifiedName, const char* namespaceURI)
{
  char *text = NULL;
  return tixiAddTextElementNS(handle, parentPath, qualifiedName, namespaceURI, text);
}


DLL_EXPORT ReturnCode tixiCreateElementAtIndex (const TixiDocumentHandle handle, const char *parentPath, const char *elementName, int index)
{
  char *text = NULL;
  return tixiAddTextElementAtIndex(handle, parentPath, elementName, text,index);
}


DLL_EXPORT ReturnCode tixiCreateElementNSAtIndex (const TixiDocumentHandle handle, const char *parentPath,
                                                  const char *qualifiedName, int index, const char* namespaceURI)
{
  char *text = NULL;
  return tixiAddTextElementNSAtIndex(handle, parentPath, qualifiedName, namespaceURI, text,index);
}


DLL_EXPORT ReturnCode tixiCheckDocumentHandle(const TixiDocumentHandle handle)
{
  TixiDocument *document = getDocument(handle);

  if (!document) {
    return INVALID_HANDLE;
  }
  else {
    return SUCCESS;
  }
}

DLL_EXPORT ReturnCode tixiCheckElement(const TixiDocumentHandle handle, const char *elementPath)
{

  TixiDocument *document = getDocument(handle);

  xmlXPathObjectPtr xpathObject = NULL;

  ReturnCode error = SUCCESS;

  if (!document) {
    printMsg(MESSAGETYPE_ERROR, "Error: Invalid document handle.\n");
    return INVALID_HANDLE;
  }

  error = checkExistence(document->xpathContext, elementPath, &xpathObject);
  xmlXPathFreeObject(xpathObject);

  return error;
}



DLL_EXPORT ReturnCode tixiCheckAttribute(TixiDocumentHandle handle, const char *elementPath, const char *attributeName)
{
  TixiDocument *document = getDocument(handle);

  char *textPtr;
  xmlNodePtr element = NULL;
  ReturnCode error = SUCCESS;

  if (!document) {
    printMsg(MESSAGETYPE_ERROR, "Error: Invalid document handle.\n");
    return INVALID_HANDLE;
  }

  error = checkElement(document->xpathContext, elementPath, &element);
  if (!error) {
    char* name = NULL;
    char* prefix = NULL;
    extractPrefixAndName(attributeName, &prefix, &name);

    if (!prefix) {
      textPtr = (char *) xmlGetProp(element, (xmlChar *) attributeName);
    }
    else {
      xmlNsPtr ns = xmlSearchNs(element->doc, element, (xmlChar *) prefix);
      if (!ns) {
        textPtr = NULL;
      }
      else {
        textPtr = (char *) xmlGetNsProp(element, (xmlChar*) name, ns->href);
      }
      free(prefix);
    }
    free(name);

    if (textPtr) {
      xmlFree(textPtr);
      return SUCCESS;
    } else {
      return ATTRIBUTE_NOT_FOUND;
    }
  } else {
    return error;
  }
}

DLL_EXPORT ReturnCode tixiXSLTransformationToFile(TixiDocumentHandle handle, const char *xslFilename, const char *resultFilename)
{
  TixiDocument *document = getDocument(handle);

  return xsltTransformToFile(document->docPtr, xslFilename, resultFilename);
}


/** -----------
 * UID Functions
 *--------------*/


DLL_EXPORT ReturnCode tixiUIDCheckDuplicates(TixiDocumentHandle handle)
{
  TixiDocument *document = getDocument(handle);

  if(!document)
    return INVALID_HANDLE;

  // if the UID list is already set, delete and rebuild it.
  if(document->uidListHead) {
    uid_clearUIDList(document);
  }

  if (uid_readDocumentUIDs(document) != SUCCESS) {
    return FAILED;
  }

  if (uid_checkForDuplicates(document) != SUCCESS) {
    return UID_NOT_UNIQUE;
  }

  return SUCCESS;
}



DLL_EXPORT ReturnCode tixiUIDCheckLinks(TixiDocumentHandle handle)
{
  TixiDocument *document = getDocument(handle);

  // if the UID list is already set, delete and rebuild it.
  if(document->uidListHead) {
    uid_clearUIDList(document);
  }

  if (uid_readDocumentUIDs(document) != SUCCESS) {
    return FAILED;
  }

  if (uid_checkForBrokenLinks(document) != SUCCESS) {
    return UID_LINK_BROKEN;
  }

  return SUCCESS;
}


DLL_EXPORT ReturnCode tixiUIDGetXPath(TixiDocumentHandle handle, const char *uID, char **xPath)
{
  ReturnCode error;
  TixiDocument *document = getDocument(handle);
  char *textPtr = NULL;

  error = tixiUIDCheckDuplicates(handle);
  if (error != SUCCESS) {
    printMsg(MESSAGETYPE_WARNING, "Warning: There are duplicated UID's in the data set!");
  }

  textPtr = (char *) uid_getXpath(document, uID);
  if ( textPtr ) {
    *xPath = (char *) malloc((strlen(textPtr) + 1) * sizeof(char));
    strcpy(*xPath, textPtr);
    xmlFree(textPtr);
  }
  else {
    *xPath = (char *) malloc(sizeof(char));
    strcpy(*xPath, "");
  }
  error = addToMemoryList(document, (void *) *xPath);
  return error;
}


DLL_EXPORT ReturnCode tixiUIDCheckExists(TixiDocumentHandle handle, const char *uID)
{
  TixiDocument *document = getDocument(handle);
  ReturnCode error = FAILED;

  if(!document)
    return INVALID_HANDLE;

  // if the UID list is already set, delete and rebuild it.
  if(document->uidListHead) {
    uid_clearUIDList(document);
  }

  if (uid_readDocumentUIDs(document) != SUCCESS) {
    return FAILED;
  }

  if (uid_checkExists(document, uID) == 0) {
    error = SUCCESS;
  } else {
    error = UID_DONT_EXISTS;
  }

  return error;
}


DLL_EXPORT ReturnCode tixiUIDSetToXPath(TixiDocumentHandle handle, const char *xPath, const char *uID)
{
  ReturnCode error;

  error = tixiUIDCheckDuplicates(handle);
  if (error != SUCCESS) {
    return error;
  }

  return tixiAddTextAttribute (handle, xPath, CPACS_UID_ATTRIBUTE_STRING, uID);
}


DLL_EXPORT ReturnCode tixiXPathEvaluateNodeNumber(TixiDocumentHandle handle, const char *xPathExpression, int *number)
{
  TixiDocument *document = getDocument(handle);
  int error = SUCCESS;

  *number = XPathGetNodeNumber(document, xPathExpression);
  if ((*number) < 0) {
    error = FAILED;
  }

  return error;
}

DLL_EXPORT ReturnCode tixiXPathExpressionGetXPath(TixiDocumentHandle handle, const char *xPathExpression, int index, char** xPath)
{
  TixiDocument *document = getDocument(handle);
  char* tmpXPath = NULL;
  int error = SUCCESS;

  if (!document) {
    printMsg(MESSAGETYPE_ERROR, "Error: Invalid document handle.\n");
    return INVALID_HANDLE;
  }

  tmpXPath = XPathExpressionGetElementPath(document, xPathExpression, index);
  if (!tmpXPath) {
    *xPath = NULL;
    error = FAILED;
  }

  // copy to output
  *xPath = buildString("%s", tmpXPath);
  free(tmpXPath);
  addToMemoryList(document, (void*) *xPath);

  return error;
}

DLL_EXPORT ReturnCode tixiXPathExpressionGetTextByIndex(TixiDocumentHandle handle, const char *xPathExpression, int elementNumber, char **text)
{
  TixiDocument *document = getDocument(handle);
  int error = SUCCESS;
  char *textPtr = NULL;

  textPtr = XPathExpressionGetText(document, xPathExpression, elementNumber);
  if ( textPtr ) {
    *text = (char *) malloc((strlen(textPtr) + 1) * sizeof(char));
    strcpy(*text, textPtr);
    error = addToMemoryList(document, (void *) *text);
  } else {
    *text = NULL;
    error = FAILED;
  }

  return error;
}

DLL_EXPORT ReturnCode   tixiGetChildNodeName(const TixiDocumentHandle handle, const char *elementPath,  int index, char **text)
{
  TixiDocument *document = getDocument(handle);
  xmlNodePtr element = NULL;
  int error = SUCCESS;

  if (!document) {
    printMsg(MESSAGETYPE_ERROR, "Error: Invalid document handle.\n");
    return INVALID_HANDLE;
  }

  if(index <= 0){
    return INDEX_OUT_OF_RANGE;
  }

  error = checkElement(document->xpathContext, elementPath, &element);

  if(!error){
    xmlNodePtr child = element->children;
    int pos = 1;

    while(child && pos < index){
      // Ignore DTD nodes
      if (child->type != XML_DTD_NODE) {
        pos++;
      }
      child = child->next;
    }

    if(pos != index || !child){
      return INDEX_OUT_OF_RANGE;
    }

    // return node value according to dom specification: http://www.w3schools.com/dom/dom_nodetype.asp
    if(child->type == XML_TEXT_NODE){
      *text = (char *) malloc(10 * sizeof(char));
      strcpy(*text, "#text");
    }
    else if(child->type == XML_CDATA_SECTION_NODE){
      *text = (char *) malloc(20 * sizeof(char));
      strcpy(*text, "#cdata-section");
    }
    else if(child->type == XML_COMMENT_NODE){
      *text = (char *) malloc(10 * sizeof(char));
      strcpy(*text, "#comment");
    }
    else {
      // get name
      *text = (char *) malloc((strlen((char*)child->name) + 3) * sizeof(char));
      strcpy(*text,  (char*) child->name);
    }
    error = addToMemoryList(document, (void *) *text);
  }
  return error;
}

DLL_EXPORT ReturnCode tixiGetNumberOfChilds(const TixiDocumentHandle handle, const char *elementPath, int* nChilds)
{
  TixiDocument *document = getDocument(handle);
  xmlNodePtr element = NULL;
  ReturnCode error = SUCCESS;


  if (!document) {
    printMsg(MESSAGETYPE_ERROR, "Error: Invalid document handle.\n");
    return INVALID_HANDLE;
  }

  error = checkElement(document->xpathContext, elementPath, &element);

  if (!error) {
    xmlNodePtr children = element->children;
    *nChilds = 0;
    while(children){
      // Ignore DTD nodes, we don't select them with xpath
      if (children->type != XML_DTD_NODE) {
        (*nChilds)++;
      }
      children = children->next;
    }
  }

  return error;
}

DLL_EXPORT ReturnCode tixiGetNumberOfAttributes(const TixiDocumentHandle handle, const char *elementPath, int* nAttributes)
{
  TixiDocument *document = getDocument(handle);
  xmlNodePtr element = NULL;
  ReturnCode error = SUCCESS;


  if (!document) {
    printMsg(MESSAGETYPE_ERROR, "Error: Invalid document handle.\n");
    return INVALID_HANDLE;
  }

  error = checkElement(document->xpathContext, elementPath, &element);

  if (!error) {
    xmlAttrPtr attr = element->properties;
    *nAttributes = 0;

    while(attr){
      attr = attr->next;
      (*nAttributes)++;
    }
  }

  return error;
}

DLL_EXPORT ReturnCode tixiGetAttributeName(const TixiDocumentHandle handle, const char *elementPath, int attrIndex, char** attrName)
{
  TixiDocument *document = getDocument(handle);
  xmlNodePtr element = NULL;
  ReturnCode error = SUCCESS;


  if (!document) {
    printMsg(MESSAGETYPE_ERROR, "Error: Invalid document handle.\n");
    return INVALID_HANDLE;
  }

  if(attrIndex <= 0){
    return INDEX_OUT_OF_RANGE;
  }

  error = checkElement(document->xpathContext, elementPath, &element);

  if (!error) {
    xmlAttrPtr attr = element->properties;
    int pos = 1;

    while(attr && pos < attrIndex){
      attr = attr->next;
      pos++;
    }

    if(pos != attrIndex || !attr){
      return INDEX_OUT_OF_RANGE;
    }

    if (attr->ns && attr->ns->prefix) {
      *attrName = (char *) malloc((strlen((char*)attr->name) + strlen((char*)attr->ns->prefix) + 4) * sizeof(char));
      sprintf(*attrName, "%s:%s", attr->ns->prefix, attr->name);
    }
    else {
      // get name
      *attrName = (char *) malloc((strlen((char*)attr->name) + 3) * sizeof(char));
      strcpy(*attrName,  (char*)attr->name);
    }
    error = addToMemoryList(document, (void *) *attrName);
  }

  return error;
}


DLL_EXPORT ReturnCode tixiGetNodeType(const TixiDocumentHandle handle, const char *nodePath, char **nodeType)
{
  TixiDocument *document = getDocument(handle);
  xmlNodePtr element     = NULL;
  int error = SUCCESS;

  if (!document) {
    printMsg(MESSAGETYPE_ERROR, "Error: Invalid document handle.\n");
    return INVALID_HANDLE;
  }

  error = checkElement(document->xpathContext, nodePath, &element);

  if (!error) {
    switch (element->type) {
    case  XML_ELEMENT_NODE:
      *nodeType = (char*) malloc ( sizeof(char)* (12 + 3));
      strcpy(*nodeType, "ELEMENT_NODE");
      break;

    case  XML_ATTRIBUTE_NODE:
      *nodeType = (char*) malloc ( sizeof(char)* (14 + 3));
      strcpy(*nodeType, "ATTRIBUTE_NODE");
      break;

    case  XML_TEXT_NODE:
      *nodeType = (char*) malloc ( sizeof(char)* (9 + 3));
      strcpy(*nodeType, "TEXT_NODE");
      break;

    case  XML_CDATA_SECTION_NODE:
      *nodeType = (char*) malloc ( sizeof(char)* (18 + 3));
      strcpy(*nodeType, "CDATA_SECTION_NODE");
      break;

    case  XML_ENTITY_REF_NODE:
      *nodeType = (char*) malloc ( sizeof(char)* (21 + 3));
      strcpy(*nodeType, "ENTITY_REFERENCE_NODE");
      break;

    case  XML_ENTITY_NODE:
      *nodeType = (char*) malloc ( sizeof(char)* (11 + 3));
      strcpy(*nodeType, "ENTITY_NODE");
      break;

    case  XML_PI_NODE:
      *nodeType = (char*) malloc ( sizeof(char)* (27 + 3));
      strcpy(*nodeType, "PROCESSING_INSTRUCTION_NODE");
      break;

    case  XML_COMMENT_NODE:
      *nodeType = (char*) malloc ( sizeof(char)* (12 + 3));
      strcpy(*nodeType, "COMMENT_NODE");
      break;

    case  XML_DOCUMENT_NODE:
      *nodeType = (char*) malloc ( sizeof(char)* (13 + 3));
      strcpy(*nodeType, "DOCUMENT_NODE");
      break;

    case  XML_DOCUMENT_TYPE_NODE:
      *nodeType = (char*) malloc ( sizeof(char)* (18 + 3));
      strcpy(*nodeType, "DOCUMENT_TYPE_NODE");
      break;

    case XML_DOCUMENT_FRAG_NODE:
      *nodeType = (char*) malloc ( sizeof(char)* (22 + 3));
      strcpy(*nodeType, "DOCUMENT_FRAGMENT_NODE");
      break;

    case  XML_NOTATION_NODE:
      *nodeType = (char*) malloc ( sizeof(char)* (13 + 3));
      strcpy(*nodeType, "NOTATION_NODE");
      break;

    default:
      *nodeType = (char*) malloc ( sizeof(char)* (12 + 3));
      strcpy(*nodeType, "UNKNOWN_NODE");
      break;
    }

    error = addToMemoryList(document, (void *) *nodeType);
  }
  return error;
}

void tixiDefaultMessageHandler(MessageType type, const char *message)
{
  // only show errors and warnings by default
  if (type < MESSAGETYPE_STATUS) {
    fputs(message, stderr);
  }
}

DLL_EXPORT ReturnCode tixiRegisterNamespace(const TixiDocumentHandle handle, const char* namespaceURI, const char* prefix)
{
  TixiDocument *document = getDocument(handle);
  int code = 0;

  if (!document) {
    printMsg(MESSAGETYPE_ERROR, "Error: Invalid document handle.\n");
    return INVALID_HANDLE;
  }

  code = XPathRegisterNamespace(document->xpathContext, namespaceURI, prefix);
  if (code == 0) {
    return SUCCESS;
  }
  else {
    return FAILED;
  }
}

DLL_EXPORT ReturnCode tixiRegisterNamespacesFromDocument(const TixiDocumentHandle handle)
{
  TixiDocument *document = getDocument(handle);
  int code = 0;

  if (!document) {
    printMsg(MESSAGETYPE_ERROR, "Error: Invalid document handle.\n");
    return INVALID_HANDLE;
  }

  code = XPathRegisterDocumentNamespaces(document->xpathContext);
  if (code == 0) {
    return SUCCESS;
  }
  else {
    return FAILED;
  }
}

DLL_EXPORT ReturnCode tixiSetElementNamespace(const TixiDocumentHandle handle, const char* elementPath, const char* namespaceURI, const char* prefix)
{
  xmlNodePtr node = NULL;
  ReturnCode retval = SUCCESS;

  TixiDocument *document = getDocument(handle);

  if (!document) {
    printMsg(MESSAGETYPE_ERROR, "Error: Invalid document handle.\n");
    return INVALID_HANDLE;
  }

  if (!namespaceURI) {
      return INVALID_NAMESPACE_URI;
  }

  retval = getNodePtrFromElementPath(handle, elementPath, &node);

  if (retval != SUCCESS) {
    return retval;
  }

  if(node == NULL) {
    return INVALID_XPATH;
  }

  if (!nodeSetNamespace(document->docPtr, node, prefix, namespaceURI)) {
    return FAILED;
  }
  else {
    return SUCCESS;
  }
}

DLL_EXPORT ReturnCode tixiDeclareNamespace(const TixiDocumentHandle handle, const char* elementPath, const char* namespaceURI, const char* prefix)
{
  xmlNodePtr node = NULL;
  ReturnCode retval = SUCCESS;

  TixiDocument *document = getDocument(handle);

  if (!document) {
    printMsg(MESSAGETYPE_ERROR, "Error: Invalid document handle.\n");
    return INVALID_HANDLE;
  }

  if (!namespaceURI) {
    printMsg(MESSAGETYPE_ERROR, "Error: Invalid namespace URI.\n");
    return INVALID_NAMESPACE_URI;
  }

  if (!prefix) {
    printMsg(MESSAGETYPE_ERROR, "Error: Empty namespace prefix.\n");
    return FAILED;
  }

  retval = getNodePtrFromElementPath(handle, elementPath, &node);

  if (retval != SUCCESS) {
    return retval;
  }

  if(node == NULL) {
    return INVALID_XPATH;
  }

  if (!nodeAddNamespace(document->docPtr, node, prefix, namespaceURI)) {
    return FAILED;
  }
  else {
    return SUCCESS;
  }
}
