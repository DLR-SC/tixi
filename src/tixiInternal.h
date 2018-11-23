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

#ifndef TIXI_INTERNAL_H
#define TIXI_INTERNAL_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file   tixiInternal.h
 * @author Hans-Peter Kersken <Hans-Peter.Kersken@dlr.de>
 * @date   Tue Mar 15 12:06:33 2005
 * @brief  Header file for routines used to implement the API.
 * This file cotains declaration for auxillary routines used to implement the tixi API.
 */
#include "libxml/parser.h"
#include "libxml/xpath.h"
#include "libxml/xmlsave.h"
#include "libxml/xpath.h"
#include "tixi.h"
#include "tixiData.h"



typedef enum {
  SUCESS,
  MEMORY_ALLOCATION_FAILED
} InternalReturnCode;

/**
 * @brief Adds documents to list of managed documents.
 *
 *
 * @param document pointer to the document to be added to the list of managed documents
 *
 * @return handle to the document
 */
InternalReturnCode addDocumentToList(TixiDocument* document, TixiDocumentHandle* handle);

/**
 * @brief Removes document from the list of managed documents.
 *
 *
 * @param document pointer to the document to be added to the list of managed documents
 *
 * @return error code
 */
ReturnCode removeDocumentFromList(TixiDocumentHandle handle);

/**
 * @brief Retrives a document.
 *
 *
 * @param (in) handle
 *
 * @return handle to the document or NULL if handle not in the list
 */
DLL_EXPORT TixiDocument* getDocument(TixiDocumentHandle handle);

/**
 @brief Adds pointer to memory allcoated by libxml to list of managed memory.

 @param (in) memory pointer to the memory location
 */
InternalReturnCode addToMemoryList(TixiDocument* document, void* memory);

/**
  @brief Frees all memory locations pointed to by the pointers in the list
         and removes all list entries.

  @param document (in) a pointer to a TixiDocument structure
 */
InternalReturnCode clearMemoryList(TixiDocument* document);

/**
  @brief Frees the memory used by the document

  @param document (in) a pointer to a TixiDocument structure
*/
void freeTixiDocument(TixiDocument* document);


/**
  @brief Checks if the given element path is valid

  @param xpathContext (in) pointer to an libxml2 xpath context
  @param elementPath (in) path to the element to be check
  @param element (out) pointer to the XML-node pointed to be element path
  @return
    - SUCCESS element exists and is unique
    - FAILED internal error
    - INVALID_XPATH
    - ELEMENT_NOT_FOUND
    - NOT_AN_ELEMENT
 */
DLL_EXPORT ReturnCode checkElement(const xmlXPathContextPtr xpathContext, const char* elementPath, xmlNodePtr* element);


/**
  @brief Checks if the given element or attribute path exists

  @param xpathContext (in) pointer to an xpath context
  @param nodePath (in) path to the element or attribute to be check
  @param node (out) pointer to the node pointed to by nodePath
  @param xpathObject (out) libxml2 internal object pointer. Has to be freed by the caller.
  @return
    - SUCCESS if node exists
    - FAILED internal error
    - INVALID_XPATH
    - ELEMENT_NOT_FOUND
 */
ReturnCode checkExistence(const xmlXPathContextPtr xpathContext, const char* nodePath, xmlXPathObjectPtr* xpathObject);


/* TODO: header! */
ReturnCode checkExternalNode(const xmlNodePtr element);

/**
  @brief Retrieves coordinate value of a point element.

  @param document (in) Pointer to the TiXI document
  @param coordinatePath (in) variable used to construct the XPath to the coordinate element
  @param pointPath (in) path to the point element
  @param index (in) XML-index of the point element in its superelement
  @param name (in) coordinate name
  @param ignoreError (in) if 1 no error will be reported if the coordinate element is not found
                          if 0 an error message is printed to stderr if the coordinate element is not found
  @param value (out) coordinate value

  @return
    - SUCCESS
    - FAILED internal error
    - INVALID_XPATH
    - ELEMENT_NOT_FOUND
    - ELEMENT_PATH_NOT_UNIQUE
    - NOT_AN_ELEMENT
    - COORDINATE_NOT_FOUND
*/

ReturnCode getCoordinateValue(TixiDocument* document, char* pointPath,
                              int pointIndex, char* name, int ignoreError, double* value);

/**
  @brief Retrieves coordinates  of a point element.

  Depending on the value of ignoreErrors the ReturnCode indicates if a coordinate element is
  missing  in an point element ( ignoreErrors = 0 ) or  missing coordinates are silently set to 0
  ( ignoreErrors = 1 ).  In the first case the routine returns after a missing coordinate is encountered.

  @param handle (in) as returned by ::tixiOpenDocument
  @param pointParentPath (in) an XPath compliant path to an element containing point elements
                            in the document specified by handle (see section \ref XPathExamples).
  @param index (in) index of the point element to be retrieved,  admissible values 1,..,n. n is the
                                 number of point elements  in the element specified by pointParentPath
  @param ignoreMissingCoordinates (in) a flag indicating if missing coordinate elements should be
                                                                        silently ignored or reported
  @param x (out) x coordinate of the point
  @param y (out) y coordinate of the point
  @param z (out) z coordinate of the point

  @return
    - SUCCESS
    - FAILED internal error
    - INVALID_XPATH
    - ELEMENT_NOT_FOUND
    - ELEMENT_PATH_NOT_UNIQUE
    - NOT_AN_ELEMENT
*/
ReturnCode getPoint(const TixiDocumentHandle handle, const char* pointParentPath, const int pointIndex,
                    int ignoreMissingCoordinates, double* x, double* y, double* z);
/**
  brief Check if libxml2 version used to build TIXI.
*/
void checkLibxml2Version();

/**
  @brief builds a string using snprintf to avoid buffer overflows. The resulting buffer
         has to be freed by the caller.
*/
DLL_EXPORT char* buildString(const char* format, ...);


/**
  @brief Open external xml files and merge them into the tree.

  External xml files could be integrated into the xml tree by linking them into the main xml file.
  The user now has only one big xml file.
  For example:

    <wings>
        <airfoils>
           <externaldata>
                <path>./WingSections/</path>
                <filename>VFW614-W-1.xml</filename>
                <filename>VFW614-W-2.xml</filename>
                ...
            </externaldata>
            <airfoil>
                <name>VFW614 Seitenleitwerksprofil</name>
                <coordinates>
                    <point><x>1.0000000</x><y>0.0000000</y><z>0.0000000</z></point>
                      <point><x>0.9795687</x><y>0.0023701</y><z>0.0000000</z></point>
                      ...
                </coordinates>
            </airfoil>
        </airfoils>
    </wings>

  @param TixiDocument (in) a TIXI document with a opened main-xml file.
  @param int (out) The number of files that where includet.

  @return
    - SUCCESS
    - FAILED internal error
    - OPEN_FAILED
*/
ReturnCode openExternalFiles(TixiDocument* aTixiDocument, int* number);


/**
  @brief Searches the tree for node that have to be saved in external files again.
         The external nodes will be removed from the main xml-document.

  @param xmlNodePtr (in) a node pointer to the starting node of the tree.
  @param TixiDocument (in) a TIXI document with a opened main-xml file.r.
  @return
    - SUCCESS all external nodes are saved successfully
    - FAILED internal error
 */
ReturnCode saveExternalFiles(xmlNodePtr aNodePrt, TixiDocument* aTixiDocument);


/**
  @brief Returns a pointer to a parent node from a given XPath.

  @param TixiDocument handle (in) a handle to a TIXI document.
  @param elementPath (in) a xpath to a xml element.
  @return
    - xmlNodePtr of the parent node
    - NULL if a error occured
 */
xmlNodePtr getParentNodeToXPath(TixiDocumentHandle handle, const char* elementPath);


/**
  @brief Adds an attribute with a string value to an element.

  Adds an attribute with name attributeName and value attributeValue
  to an element specified by the elementPath expression. If the
  attribute already exists its previous value is replaced by text.

  @param xpathContext (in) libxml2 pointer to the xpath context.
  @param elementPath (in) a xpath to a xml element.
  @param attributeName (in) the name of the attribute to set.
  @param attributeValue (in) the value of the attribute to set.
  @return
    - NO_ATTRIBUTE_NAME
    - INVALID_XML_NAME
    - SUCCESS
 */
ReturnCode genericAddTextAttribute(xmlXPathContextPtr xpathContext, const char* elementPath,
                                   const char* attributeName, const char* attributeValue);



/**
  @brief Returns a pointer to a node from a given XPath.

  @param TixiDocument handle (in) a handle to a TIXI document.
  @param elementPath (in) a xpath to a xml element.
  @param xmlNodePtr (out) xmlNodePtr of the parent node

  @return ReturnCode
    - SUCCESS
    - INVALID_HANDLE
    - ALREADY_SAVED
    - INVALID_XPATH
    - ELEMENT_NOT_FOUND
    - ELEMENT_PATH_NOT_UNIQUE
    - FAILED
 */
ReturnCode getNodePtrFromElementPath(TixiDocumentHandle handle, const char* elementPath, xmlNodePtr* nodePtr);


/**
  @brief Creates a new TIXI Document and copies the XML Document from
         a given old TIXI Document to it.

  @param TixiDocument oldTixiDocumentHandle (in) The source TIXIDocument
  @param TixiDocument newTixiDocumentHandle (in) The new created destination TIXIDocument
  @return int
    - 0 if SUCCESS
    - != 0 if ERROR
 */
int copyDocument(const TixiDocumentHandle oldTixiDocumentHandle, TixiDocumentHandle* newTixiDocumentHandle);


/**
  @brief Saves a TIXI document to file depending on the InterSaveMode.

  @param TixiDocument TixiDocumentHandle (in) The TIXIDocument
  @param char* xmlFilename (in) filename of for the main xml document
  @param InternalSaveMode saveMode (in) the mode in which to save
  @return ReturnCode
    - SUCCESS              if the file is successfully written and validated
    - INVALID_HANDLE       if handle not found in list of man
    - FAILED               if writing and closing the XML-file failed
 */
ReturnCode saveDocument (TixiDocumentHandle handle, const char* xmlFilename, InternalSaveMode saveMode);


/**
  @brief Validates a tixiDocument against a schema stored in a xmlDocPtr.

  @param TixiDocument TixiDocumentHandle (in) The TIXIDocument
  @param xmlDocPtr *schema_doc (in) DocPtr with the schema to validate against
  @param int withDefaults (in) a flag indicating that default elements/attributes in the schema
                                                            should be added to the tixiDocument
  @return ReturnCode
    - SUCCESS              if the document is successfully validated
    - NOT_WELL_FORMED      if the XML-document is not well formed
    - NOT_SCHEMA_COMPLIANT if the XML-document is well-formed
                           but validating against the given XML-schema fails
    - OPEN_SCHEMA_FAILED   if opening of the XML-schema-file failed
    - FAILED               for all internal errors
 */
ReturnCode validateSchema(const TixiDocumentHandle handle, xmlDocPtr* schema_doc, int withDefaults);


/**
  @brief Generates an XPath from a given nodePtr containing all uIDs and indices.
         The memory used for the string is allocated by tixi and is must not be removed
         from the user.

  @param TixiDocument TixiDocumentHandle (in) The TIXIDocument
  @param xmlNodePtr *aNodePrt (in) NodePrt of the node to generate the XPath from
  @return ReturnCode
    - char*                    The string with the generated XPath
    - NULL                     If an error occurred
 */
DLL_EXPORT char* generateXPathFromNodePtr(TixiDocumentHandle handle, xmlNodePtr aNodePrt);


/**
  @brief Reorders nodes of a nodeSet. The node at the position fromIndex will be put in the position toIndex.
         All nodes between that two nodes will be reordered.


  @param handle (in) a valid tixi handle
  @param elementPath (in) path to the element to be reordered
  @param int fromIndex (in) Position of the starting node that should reordered. If -1, that last node will be assumed.
  @param int toIndex (in) New position of the node that should reordered. If -1, that last node will be assumed.
  @return ReturnCode
    - SUCCESS                    If the reordering went well
    - FAILED                     If fromIndex or toIndex is out of range.
 */
ReturnCode reorderXmlElements(TixiDocumentHandle handle, const char* elementPath, int fromIndex, int toIndex);

/**
 * @brief Tests, whether p is parent of n
 * 
 * @return 1, if p is parent of n, otherwise 0
 */
int isParent(xmlNodePtr p, xmlNodePtr n);

#ifdef __cplusplus
}
#endif

#endif /* TIXI_INTERNAL_H */
