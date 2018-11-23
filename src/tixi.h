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
#ifdef __cplusplus
extern "C" {
#endif

#if defined(WIN32)
#if defined (tixi3_EXPORTS)
#define DLL_EXPORT __declspec (dllexport)
#else
#define DLL_EXPORT
#endif
#else
#define DLL_EXPORT
#endif

/* \mainpage TIXI Manual
<b>Contents:</b>
  - \link Glossary Glossary\endlink
  - \link  XPathExamples XPath Examples\endlink
  - \link UsageExamples Usage\endlink
  - <a HREF="modules.html"  class="el">Function Documentation</a>
 */
/*
 @file   tixi.h
 @author Hans-Peter Kersken <Hans-Peter.Kersken@dlr.de>
         Markus Litz <Markus.Litz@dlr.de>,
         Markus Kunde <Markus.Kunde@dlr.de>,
         Arne Bachmann <Arne.Bachmann@dlr.de>,
 @date   Tue Feb 02 14:28:05 2009

 @brief  Definition of  <b>TIXI</b> the <b>TI</b>VA - <b>XML</b> - <b>I</b>nterface
*/
/*! \page Glossary Glossary

 XML-file: file containing an XML-document

 XML-document: abstraction which either refers to the contents of an
 XML-file or to the data structure held in memory.
*/
/*! \page XPathExamples XPath Examples

 The string to describe a path to an element has to be an XPath
 expression but only a very restricted subset of XPath expressions
 are allowed: On retrieving information the specified path has to be
 unique. If an element name appears more than once inside the same
 parent element (see wings/wing in the example) the expression has
 to be contain an index to denominate exactly which element has to
 be used. As long as these restrictions are met any syntactically
 correct XPath expression is allowed. When adding an element the
 complete path has to be specified.

 Example:
 @verbatim
 <?xml version="1.0" encoding="UTF-8"?>
 <plane>
     <name>Junkers JU 52</name>
     <wings>
         <wing position='left'>
             <centerOfGravity referenceSystem='relative'>
                 <x unit="m">30.0</x>
                 <y unit="m">10.0</y>
                 <z unit="m">5.0</z>
             </centerOfGravity>
         </wing>
         <wing position='right'>
             <centerOfGravity referenceSystem='relative'>
                 <x unit="m">30.0</x>
                 <y unit="m">-10.0</y>
                 <z unit="m">5.0</z>
             </centerOfGravity>
         </wing>
     </wings>
     <coordinateOrigin>
         <x unit="m">0.0</x>
         <y unit="m">0.0</y>
         <z unit="m">0.0</z>
     </coordinateOrigin>
 </plane>
@endverbatim

 The path to the x coordinate value of the center of gravity of the
 plane is:

 /plane/centerOfGravity/x

 The path to the x coordinate value of the center of gravity of the
 first wing element is:

 /plane/wings/wing[1]/centerOfGravity/x

 The path to the name can either by expressed as

 /plane/name

 or

 //name

 which would select all elements called name but as long as this element name is
 unique in the document it is valid in the context of TIXI.

 None valid expressions are:

 /plane/wings/wing/centerOfGravity/z is not unique because there are two wing elements in wings.

 /plane/coordinateOrigin specifies an element with children containing text.

 //x is not unique because it would select all elements named x as there are:
  -  /plane/wings/wing[1]/centerOfGravity/x
  -  /plane/wings/wing[2]/centerOfGravity/x
  -  /plane/centerOfGravity/x
*/
/**
 @page UsageExamples Usage

 The following exmaple assume the XML example from section \ref XPathExamples to be stored in a file named ju.xml.

 - Retrieve the position of the first wing.

@verbatim
 #include "tixi.h"

 char* xmlFilename = "ju.xml";
 TixiDocumentHandle handle = -1;
 char * elementPath = "/plane/wings/wing[1]";
 char* attributeName = "position";

 char* attributeValue;

 tixiOpenDocument( xmlFilename, &handle );
 tixiGetTextAttribute( handle, elementPath, attributeName, &attributeValue );
 tixiCloseDocument( handle );
@endverbatim

 - Retrieve x value of the coordinate origin.

@verbatim
 #include "tixi.h"

 char* xmlFilename = "ju.xml";
 TixiDocumentHandle handle = -1;
 char * elementPath = "/plane/coordinateOrigin/x";
 double x = 0.;

 tixiOpenDocument( xmlFilename, &handle );
 tixiGetDoubleElement( handle, elementPath, &x );
 tixiCloseDocument( handle );
@endverbatim
*/
/**
 \page Fortran Notes for Fortran programming

  The Fortran interface is implemented by calls to subroutines.

  It assumes the follwing mapping for the basic types:

  real is real*8 corresponds double

  integer is integer*4 corresponds to int

  character corresponds char

  Character strings are to be passed as variables of type
  character*N. If a string is returned by a subroutine call the
  variable holding the result must be large enough to hold the result.
  Otherwise the result is truncated and the return code is set to
  STRING_TRUNCATED.

  NOTE: In view of these restrictions an implementation using
  character arrays should be considered.

  The return codes returned in the last argument corresponds to their
  position in ::ReturnCode starting with 0. A routine will be supplied
  to directly access the meaning of a return code as a string.

  When the C interface requires to pass a NULL-pointer, e.g. to choose
  the default format "%g" when writing floating point elements, the
  respective argument in the Fortran interfaces is the empty string
  constant "". This is the only way to represent a string of length
  zero. Passing a variable with all characters set to "" will, via the
  interface transformed into an emtpy C-string "\0", which is of
  length 1, and not to a NULL-pointer.

 @page Matlab Using the TiXI MATLAB interface

  The TiXI binary distribution includes interfaces to the MATLAB language. On Windows systems, we ship
  with a precompiled MEX file and script files that can be found under share/tixi/matlab.

  On Linux, we cannot provide precompiled binaries of the interface. Instead you can find all
  script files and the MEX input file tiximatlab.c under share/tixi/matlab. To compile the
  MATLAB bindings, the tool "mex" is required, which is typically part of each MATLAB installation.
  To compile, use our Makefile by typing in the command "make".
*/


/**
 @internal
 @section ImplementationIssuues Implementation issues

  - Memory allocated by TIXI and associated with a document is released
   when closing the document.

  - TixiDocumentHandle is an integer used as index to access an TIXI
   internal data structure.

 @todo Add for final version:
 @todo - addNamespace
 @todo - check for attribute/elemet name not NULL in arguments passed to respective routines
 @todo - Fortran90, FORTRAN77 interface testing and return code checking

 @todo - when adding elements, created necessary parent elements which do not already exist too.
*/

#ifndef TIXI_H
#define TIXI_H

/**
   Datatype for TixiDocumentHandle.
*/
typedef int TixiDocumentHandle;


/**
  \defgroup Enums Enumerations
*/

/**
    \ingroup Enums
    ReturnCode return code of TIXI-routines.
     Has a typedef to ReturnCode.
*/
enum ReturnCode
{
  SUCCESS,                    /*!< 0: No error occurred                         */

  FAILED,                     /*!< 1: Unspecified error                         */

  INVALID_XML_NAME,           /*!< 2: Non XML standard compliant name specified */

  NOT_WELL_FORMED,            /*!< 3: Document is not well formed               */

  NOT_SCHEMA_COMPLIANT,       /*!< 4: Document is not schema compliant          */

  NOT_DTD_COMPLIANT,          /*!< 5: Document is not DTD compliant             */

  INVALID_HANDLE,             /*!< 6: Document handle is not valid              */

  INVALID_XPATH,              /*!< 7: XPath expression is not valid             */

  ELEMENT_NOT_FOUND,          /*!< 8: Element does not exist in document        */

  INDEX_OUT_OF_RANGE,         /*!< 9: Index supplied as argument is not
                                   inside the admissible range               */

  NO_POINT_FOUND,             /*!< 10: No point element found a given XPath      */

  NOT_AN_ELEMENT,             /*!< 11: XPath expression does not point to an
                                   XML-element node                          */

  ATTRIBUTE_NOT_FOUND,        /*!< 12: Element does not have the attribute       */

  OPEN_FAILED,                /*!< 13: Error on opening the file                 */

  OPEN_SCHEMA_FAILED,         /*!< 14: Error on opening the schema file          */

  OPEN_DTD_FAILED,            /*!< 15: Error on opening the DTD file             */

  CLOSE_FAILED,               /*!< 16: Error on closing the file                 */

  ALREADY_SAVED,              /*!< 17: Trying to modify already saved document   */

  ELEMENT_PATH_NOT_UNIQUE,    /*!< 18: Path expression can not be resolved
                                   unambiguously                             */

  NO_ELEMENT_NAME,            /*!< 19: Element name argument is NULL             */

  NO_CHILDREN,                /*!< 20: Node has no children                      */

  CHILD_NOT_FOUND,            /*!< 21: Named child is not child of element
                                   specified                                 */

  EROROR_CREATE_ROOT_NODE,    /*!< 22: Error when adding root node to new
                                   document                                  */

  DEALLOCATION_FAILED,        /*!< 23: On closing a document the
                                   deallocation of allocated memory fails    */

  NO_NUMBER,                  /*!< 24: No number specified                       */

  NO_ATTRIBUTE_NAME,          /*!< 25: No attribute name specified               */

  STRING_TRUNCATED,           /*!< 26: String variable supplied is to small to
                                   hold the result, Fortran only             */

  NON_MATCHING_NAME,          /*!< 27: Row or column name specified do not
                                   match the names used in the document      */

  NON_MATCHING_SIZE,          /*!< 28: Number of rows or columns specified do
                                   not match the sizes of the matrix in the
                                   document                                  */

  MATRIX_DIMENSION_ERROR,     /*!< 29: if nRows or nColumns or both are
                                   less than 1 */

  COORDINATE_NOT_FOUND,       /*!< 30: missing coordinate inside a point element */

  UNKNOWN_STORAGE_MODE,        /*!< 31: storage mode specified is neither
                                   ROW_WISE nor COLUMN_WISE  */

  UID_NOT_UNIQUE,                /*!< 32: One or more uID's are not unique */

  UID_DONT_EXISTS,                /*!< 33: A given uID's does not exist */

  UID_LINK_BROKEN,                /*!< 34: A node the is specified as a Link has no correspoding uid in that data set */

  INVALID_NAMESPACE_URI,          /*!< 35: The namespace URI is invalid (e.g. a null pointer) */

  INVALID_NAMESPACE_PREFIX        /*!< 36: The namespace prefix is invalid */

};

typedef enum ReturnCode ReturnCode;

/**

 \ingroup Enums
      Strorage mode of arrays This enum indicates how a matrix is stored
      in an array on input or should be retrieved into an array on
      output. If ROW_WISE is specified the order will be (1,1), (1,2),
      ..., (2,1), (2,2), (2,3)... If COLUMN_WISE is specified the order
      will be (1,1), (2,1), (3,1), .., (1,2),(2,2),(3,2) ...

  Has a typedef to StorageMode.
   */
enum StorageMode
{
  ROW_WISE,                   /*!< row wise order                             */
  COLUMN_WISE                 /*!< column wise                                */
};


typedef enum StorageMode StorageMode;


/**

 \ingroup Enums
      Open mode of a xml file
      This enum indicates how a xml file is opend.
      If OpenMode is OPENMODE_PLAIN, the xml file is open "normal" and just the given
      file is opend. If OpenMode is OPENMODE_RECURSIVE, then all external files
      specified in a <externaldata> node are opend and replaced in the xml tree.

  Has a typedef to OpenMode.
   */
enum OpenMode
{
  OPENMODE_PLAIN,                   /*!< Open just the xml file       */
  OPENMODE_RECURSIVE                /*!< Open with external files     */
};


typedef enum OpenMode OpenMode;

/**

  \ingroup Enums
       Type of a TiXI message

       This is used, if the user wants to override the default message system
       in TiXI.
    */
enum MessageType
{
  MESSAGETYPE_ERROR,                 /*!< The message is an error      */
  MESSAGETYPE_WARNING,               /*!< The message is a warning     */
  MESSAGETYPE_STATUS                 /*!< A status message             */
};


typedef enum MessageType MessageType;

/**
 * TixiPrintMsgFnc:
 * @param[in]  type The message type (error, warning, status)
 * @param[in]  msg The message
 * @param[in]  ... extra arguments
 *
 * Signature of a callback function to handle messages (errors, warnings ...)
 * To be used in conjuction with ::tixiSetPrintMsgFunc.
 */
typedef void (*TixiPrintMsgFnc) (MessageType type, const char *msg);

/**
  @brief Returns the version number of this TIXI version.

  <b>Fortran syntax:</b>
  tixi_get_version( character version )

  @return
      - char* A string with the version number.
*/
DLL_EXPORT char* tixiGetVersion();



/**
  \defgroup FileHandling File Handling Functions
    Function to open, create, close, and save XML-files.
 */
/*@{*/
/**
  @brief Open an XML-file for reading.

  Opens an XML-file specified by xmlFilename for reading and checks
  if it is well formed. To validate the document against a XML-Schema
  or DTD use ::tixiSchemaValidateFromFile, ::tixiSchemaValidateFromString or ::tixiDTDValidate.


  <b>Fortran syntax:</b>

  tixi_open_document( character*n xml_filename, integer handle, integer error )

  @param[in]  xmlFilename name of the XML-file to be opened
  @param[out] handle  handle to the XML-document. This handle is used in
                      calls to other TIXI functions.
  @return

    - SUCCESS if successfully opened the XML-file
    - NOT_WELL_FORMED if opening the XML-file succeeds but test for
                      well-formedness fails
    - OPEN_FAILED if opening of the XML-file failed
 */

DLL_EXPORT ReturnCode tixiOpenDocument (const char *xmlFilename, TixiDocumentHandle * handle);



/**
  @brief Open an XML-file for reading. It acts like tixiOpenDocument.

  If OpenMode is OPENMODE_PLAIN, only the given xml is opened. If OpenMode is
  OPENMODE_RECURSIVE, external xml files will be integrated into the xml tree
  by linking them into the main xml file. The user now has only one big xml file.
  In the path node has be be a valid URI. This uri could adress a relativ or absolut file path,
  or a http url. Example values for the path node are:
    - absolute local directory: "file:///tmp/" or "file:///c:/windws/"
    - relative local directory: "file://relativeDirectory/" or "file://../anotherRelativeDirectory/"
    - remote http ressource: "http://www.someurl.de/"

  Examples for the externaldata node:
  @code{.xml}
  <wings>
      <airfoils>
              <externaldata>
                  <path>file://aDirectory/</path>
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
  @endcode

  <b>Fortran syntax:</b>

  tixi_open_document_recursive( character*n xml_filename, integer handle, integer openmode, integer error )

  @param[in]  xmlFilename name of the XML-file to be opened
  @param[out] handle      handle to the XML-document. This handle is used in
                          calls to other TIXI functions.
  @param[in]  oMode       Enum of the mode to open (OPENMODE_PLAIN / OPENMODE_RECURSIVE).

  @return
    - SUCCESS if successfully opened the XML-file
    - NOT_WELL_FORMED if opening the XML-file succeeds but test for
                      well-formedness fails
    - OPEN_FAILED if opening of the XML-file failed
 */
DLL_EXPORT ReturnCode tixiOpenDocumentRecursive (const char *xmlFilename, TixiDocumentHandle * handle, OpenMode oMode);


/**
  @brief Open an XML-file for reading from a http web resource.

  Opens an XML-file specified by httpURL for reading and checks
  if it is well formed. To validate the document against a XML-Schema
  or DTD use ::tixiSchemaValidateFromFile, ::tixiSchemaValidateFromString or ::tixiDTDValidate.


  <b>Fortran syntax:</b>

  tixi_open_document_from_http( character*n xml_httpurl, integer handle, integer error )

  @param[in]  httpURL url of the XML-file to be opened
  @param[out] handle  handle to the XML-document. This handle is used in
                      calls to other TIXI functions.

  @return
    - SUCCESS if successfully opened the XML-file
    - NOT_WELL_FORMED if opening the XML-file succeeds but test for
                          well-formedness fails
    - OPEN_FAILED if opening of the XML-file failed
 */
DLL_EXPORT ReturnCode tixiOpenDocumentFromHTTP (const char *httpURL, TixiDocumentHandle * handle);

/**

  @brief Create an XML-document.

  Initializes a data structure to hold an XML-document.

  <b>Fortran syntax:</b>

  tixi_create_document( character*n root_element_name, integer handle, integer error )

  @param[in]  rootElementName  name of the root element of the XML-document
  @param[out] handle           handle to an XML-document. This handle is
                               used in calls to other TIXI functions.

  @return
    - SUCCESS if data structure is set-up successfully
    - FAILED if data structure could not created
 */
DLL_EXPORT ReturnCode tixiCreateDocument (const char *rootElementName, TixiDocumentHandle * handle);

/**
  @brief Returns the file path to the document.

  The path is empty, if the document was not opened, but created by ::tixiCreateDocument.

  @param[in]  handle document handle as returned by ::tixiOpenDocument, ::tixiOpenDocumentRecursive, ::tixiOpenDocumentFromHTTP, ::tixiCreateDocument or ::tixiImportFromString
  @param[out] documentPath  Path to the file, opened by ::tixiOpenDocument, ::tixiOpenDocumentRecursive, ::tixiOpenDocumentFromHTTP
                            The path is a null pointer, if the document was created by ::tixiCreateDocument or ::tixiImportFromString

  @return
    - SUCCESS in case of no errors.
    - FAILED if documentPath is a null pointer.
    - INVALID_HANDLE if the document handle is invalid.
 */
DLL_EXPORT ReturnCode tixiGetDocumentPath (TixiDocumentHandle handle, char** documentPath);

/**
  @brief Write XML-document to disk.

  The document is written into a file specified by xmlFilename. The
  user should validate the document before it is written to
  disk. Memory allocated internally for processing this document has to
  be released by ::tixiCloseDocument.

  If the file was opened with OpenMode=OPENMODE_RECURSIVE and external file where linked into
  the main XML tree, please note that these files are saved back into the external files. The external
  nodes will not be removed from the main xml-document.

  <b>Fortran syntax:</b>

  tixi_save_document( integer  handle, character*n xml_filename, integer error )


  @param[in]  handle document handle as returned by ::tixiOpenDocument, ::tixiOpenDocumentRecursive, ::tixiOpenDocumentFromHTTP, ::tixiCreateDocument or ::tixiImportFromString
  @param[in]  xmlFilename name of the file to be created.

  @return
    - SUCCESS        if the file is successfully written and validated
    - INVALID_HANDLE if  handle not found in list of man
    - FAILED         if writing and closing the XML-file failed
*/
DLL_EXPORT ReturnCode tixiSaveDocument (TixiDocumentHandle handle, const char *xmlFilename);


/**
  @brief Write XML-document with all external data to disk.

  The document is written into a file specified by xmlFilename. The
  user should validate the document before it is written to
  disk. Memory allocated internally for processing this document has to
  be released by ::tixiCloseDocument.

  If the file was opened with OpenMode=OPENMODE_RECURSIVE and external file where linked into
  the main XML document, these additional nodes will be saved in the xml file as well. The external
  nodes will not be removed from the main xml-document.

  <b>Fortran syntax:</b>

  tixi_save_complete_document( integer  handle, character*n xml_filename, integer error )


  @param[in]  handle document handle as returned by ::tixiOpenDocument, ::tixiOpenDocumentRecursive, ::tixiOpenDocumentFromHTTP, ::tixiCreateDocument or ::tixiImportFromString
  @param[in]  xmlFilename name of the file to be created.

  @return
    - SUCCESS              if the file is successfully written and validated
    - INVALID_HANDLE if  handle not found in list of man
    - FAILED         if writing and closing the XML-file failed
*/

DLL_EXPORT ReturnCode tixiSaveCompleteDocument (TixiDocumentHandle handle, const char *xmlFilename);


/**
  @brief Write XML-document with all external data to disk.

  The document is written into a file specified by xmlFilename. The
  user should validate the document before it is written to
  disk. Memory allocated internally for processing this document has to
  be released by ::tixiCloseDocument.

  If the file was opened with OpenMode=OPENMODE_RECURSIVE and external file where linked into
  the main XML document, these additional nodes will be saved in the xml file as well. The external
  nodes will be removed from the main xml-document.

  <b>Fortran syntax:</b>

  tixi_save_and_remove_document( integer  handle, character*n xml_filename, integer error )


  @param[in]  handle document handle as returned by ::tixiOpenDocument, ::tixiOpenDocumentRecursive, ::tixiOpenDocumentFromHTTP, ::tixiCreateDocument or ::tixiImportFromString
  @param[in]  xmlFilename name of the file to be created.

  @return
    - SUCCESS              if the file is successfully written and validated
    - INVALID_HANDLE if  handle not found in list of man
    - FAILED         if writing and closing the XML-file failed
  */
DLL_EXPORT ReturnCode tixiSaveAndRemoveDocument (TixiDocumentHandle handle, const char *xmlFilename);


/**
  @brief Close an XML-document.

  Closes an XML-document. This routine should be called after the
  processing of an XML-document is completed. After calling this
  routine the handle is invalid and no further processing of the
  document is possible. It must be called after ::tixiSaveDocument.

  <b>Fortran syntax:</b>

  tixi_close_document( integer  handle, integer error )

  @param[in]  handle file handle as returned by ::tixiOpenDocument, ::tixiOpenDocumentRecursive, ::tixiOpenDocumentFromHTTP, ::tixiCreateDocument or ::tixiImportFromString

  @return
    - SUCCESS if successfully closed the XML-file
    - INVALID_HANDLE if  handle not found in list of man
    - CLOSE_FAILED if closing  the XML-file failed
 */

DLL_EXPORT ReturnCode tixiCloseDocument (TixiDocumentHandle handle);


/**
  @brief Close all open documents.

  Closes all XML-documents. This routine could be called at the
  of a program to free allocated memory. After calling this
  routine all current handles are invalid and no further processing of the
  document is possible.

  <b>Fortran syntax:</b>

  tixi_close_all_documents( integer error )

  @return
    - SUCCESS if successfully closed the XML-file
    - CLOSE_FAILED if closing  the XML-files failed
 */
DLL_EXPORT ReturnCode tixiCloseAllDocuments ();

/**
  @brief Closes the xml2 library and frees its allocated variables
 */
DLL_EXPORT ReturnCode tixiCleanup ();


/**
  @brief Returns the Document as one text.

  Returns the text content of the document specified by handle.
  If an error occurs text is set to NULL. On successful return the memory used for text is allocated
  internally and must not be released by the user. The deallocation
  is handle when the document referred to by handle is closed.
  If OpenMode=OPENMODE_RECURSIVE, text will be one big string with all external
  xml files included.

  <b>Fortran syntax:</b>

  tixi_export_document_as_string( integer handle, character*n text, integer error )

  @param[in]  handle handle as returned by ::tixiOpenDocument, ::tixiOpenDocumentRecursive, ::tixiOpenDocumentFromHTTP, ::tixiCreateDocument or ::tixiImportFromString

  @param[out] text text content of the document

  @return
    - SUCCESS if successfully retrieve the text content of a single element
    - INVALID_HANDLE if the handle is not valid, i.e.  does not or no longer exist
 */
DLL_EXPORT ReturnCode tixiExportDocumentAsString (const TixiDocumentHandle handle, char **text);


/**
  @brief Imports a char-string into a new tixi-document.

  Creates a new TIXI-document with the content of the string and checks
  if it is well formed. To validate the document against a XML-Schema
  or DTD use ::tixiSchemaValidateFromFile, ::tixiSchemaValidateFromString or ::tixiDTDValidate.

  <b>Fortran syntax:</b>

  tixi_import_from_string( character*n xmlImportString, integer handle, integer error )

  @param[in]  xmlImportString the string with the xml-content
  @param[out] handle  handle to the XML-document. This handle is used in
                      calls to other TIXI functions.

  @return
    - SUCCESS if successfully imported the string
    - NOT_WELL_FORMED if importing of the string succeeds but test for well-formedness fails
 */
DLL_EXPORT ReturnCode tixiImportFromString (const char *xmlImportString, TixiDocumentHandle * handle);

/*@}*/
/**
  \defgroup Validation Validation Functions

   Functions to validate document with respect to DTD or Schemas.
 */
/*@{*/


/**
  @brief Validate XML-document against an XML-schema.

  Validates an XML-document against an XML-schema specified by
  xsdFilename. This routine should be called after opening a
  document by ::tixiOpenDocument and before ::tixiSaveDocument if the
  validation against an XML-schema is desired for the input file and
  the output file, respectively.

  <b>Fortran syntax:</b>

  tixi_schema_validate_from_file( integer  handle, character*n xsd_filename, integer error )

  @param[in]  xsdFilename name of the XML-schema-file to be used.

  @param[in]  handle handle to the XML-document.

  @return
    - SUCCESS              if the document is successfully validated
    - NOT_WELL_FORMED      if the XML-document is not well formed
    - NOT_SCHEMA_COMPLIANT if the XML-document is well-formed
                           but validating against the given XML-schema fails
    - OPEN_SCHEMA_FAILED   if opening of the XML-schema-file failed
    - FAILED               for all internal errors
 */
DLL_EXPORT ReturnCode tixiSchemaValidateFromFile (const TixiDocumentHandle handle, const char *xsdFilename);

/**
  @brief Validate XML-document against an XML-schema and insert missing default elements and attributes.

  Validates an XML-document against an XML-schema specified by
  xsdFilename. This routine should be called after opening a
  document by ::tixiOpenDocument and before ::tixiSaveDocument if the
  validation against an XML-schema is desired for the input file and
  the output file, respectively.
  If the schema contains default values, these are added to the
  XML-document (which is not the case for tixiSchemaValidateFromFile).

  <b>Fortran syntax:</b>

  tixi_schema_validate_with_defaults_from_file( integer  handle, character*n xsd_filename, integer error )

  @param[in]  xsdFilename name of the XML-schema-file to be used.

  @param[in]  handle handle to the XML-document.

  @return
    - SUCCESS              if the document is successfully validated
    - NOT_WELL_FORMED      if the XML-document is not well formed
    - NOT_SCHEMA_COMPLIANT if the XML-document is well-formed
                           but validating against the given XML-schema fails
    - OPEN_SCHEMA_FAILED   if opening of the XML-schema-file failed
    - FAILED               for all internal errors
 */
DLL_EXPORT ReturnCode tixiSchemaValidateWithDefaultsFromFile (const TixiDocumentHandle handle, const char *xsdFilename);

/**
  @brief Validate XML-document against an XML-schema.

  Validates an XML-document against an XML-schema specified by
  xsdString. This routine should be called after opening a
  document by ::tixiOpenDocument and before ::tixiSaveDocument if the
  validation against an XML-schema is desired for the input file and
  the output file, respectively.
  The complete schema has to be in the string xsdString before running
  this function.

  <b>Fortran syntax:</b>

  tixi_schema_validate_from_string( integer  handle, character*n xsd_string, integer error )

  @param[in]  xsdString char array witch is holding a schema.

  @param[in]  handle handle to the XML-document.

  @return
    - SUCCESS              if the document is successfully validated
    - NOT_WELL_FORMED      if the XML-document is not well formed
    - NOT_SCHEMA_COMPLIANT if the XML-document is well-formed
                           but validating against the given XML-schema fails
    - OPEN_SCHEMA_FAILED   if opening of the XML-schema-file failed
    - FAILED               for all internal errors
 */
DLL_EXPORT ReturnCode tixiSchemaValidateFromString (const TixiDocumentHandle handle, const char *xsdString);


/**
  @brief Validate XML-document against a DTD.

  Validates an XML-document against a DTD specified by
  DTDFilename. This routine is to be called after opening a
  document by ::tixiOpenDocument and before ::tixiSaveDocument if the
  validation against a DTD is desired for the input file and the
  output file, respectively.

  <b>Fortran syntax:</b>

  tixi_dtd_validate( integer  handle, character*n dtd_filename, integer error )

  @param[in]  DTDFilename name of the DTD-file to be used

  @param[in]  handle handle to the XML-document to be validated.

  @return
    - SUCCESS              if the document is successfully validated
    - NOT_WELL_FORMED      if the XML-document is not well formed
    - NOT_DTD_COMPLIANT    if the XML-document is well-formed but validating
                           against the given DTD fails
    - OPEN_DTD_FAILED      if opening of the DTD-file failed
 */
DLL_EXPORT ReturnCode tixiDTDValidate (const TixiDocumentHandle handle, const char *DTDFilename);
/*@}*/



/**
  \defgroup Elements Element handling functions
  Functions to get the content of an element as a string or a number, functions
  to create and manipulate its content, and a funtion to remove an element are described
  in this section.
 */

/*@{*/
/**
  @brief Retrieve text content of an element.

  Returns the text content of the element specified by elementPath in the
  document specified by handle. elementPath must refer to exactly one
  element which has only a text node and zero or more attributes but
  no further children with text nodes. If an error occurs text is set
  to NULL. On successful return the memory used for text is allocated
  internally and must not be released by the user. The deallocation
  is handle when the document referred to by handle is closed.

  <b>Fortran syntax:</b>

  tixi_get_text_element( integer handle, character*n element_path,  character*n text, integer error )

  @param[in]  handle handle as returned by ::tixiOpenDocument, ::tixiOpenDocumentRecursive or ::tixiOpenDocumentFromHTTP

  @param[in]  elementPath an XPath compliant path to an element in the document
                    specified by handle (see section \ref XPathExamples above).

  @param[out] text text content of the element specified by elementPath

  @return
    - SUCCESS if successfully retrieve the text content of a single element
    - INVALID_HANDLE if the handle is not valid, i.e.  does not or no longer exist
    - INVALID_XPATH if elementPath is not a well-formed XPath-expression
    - ELEMENT_NOT_FOUND if elementPath does not point to a node in the XML-document
    - ELEMENT_PATH_NOT_UNIQUE if elementPath resolves not to a single element but to a list of elements
 */
DLL_EXPORT ReturnCode tixiGetTextElement (const TixiDocumentHandle handle,
                                          const char *elementPath, char **text);


/**
  @brief Retrieve integer content of an element.

  Returns the content of the element specified by elementPath in the
  document specified by handle as an integer. elementPath must refer to exactly one
  element which has only a text node and zero or more attributes but
  no further children with text nodes. If an error occurs text is set
  to NULL. On successful return the memory used for text is allocated
  internally and must not be released by the user. The deallocation
  is handle when the document referred to by handle is closed.

  <b>Fortran syntax:</b>

  tixi_get_integer_element( integer  handle, character*n element_path, int* number, integer error )

  @param[in]  handle handle as returned by ::tixiOpenDocument, ::tixiOpenDocumentRecursive or ::tixiOpenDocumentFromHTTP

  @param[in]  elementPath an XPath compliant path to an element in the document
                    specified by handle (see section \ref XPathExamples above).

  @param[out] number  content of the element specified by elementPath interpreted as an integer number

  @return
    - SUCCESS if successfully retrieve the text content of a single element
    - INVALID_HANDLE if the handle is not valid, i.e.  does not or no longer exist
    - INVALID_XPATH if elementPath is not a well-formed XPath-expression
    - ELEMENT_NOT_FOUND if elementPath does not point to a node in the XML-document
    - ELEMENT_PATH_NOT_UNIQUE if elementPath resolves not to a single element but to a list of elements
 */
DLL_EXPORT ReturnCode tixiGetIntegerElement (const TixiDocumentHandle handle, const char *elementPath, int *number);

/**
  @brief Retrieve floating point content of an element.

  Returns the content of the element specified by elementPath in the
  document specified by handle as a floating point
  number. elementPath must refer to exactly one element which has
  only a text node and zero or more attributes but no further
  children with text nodes. If an error occurs number is set to
  NULL.

  <b>Fortran syntax:</b>

  tixi_get_double_element( integer  handle, character*n element_path, real number, integer error )

  @param[in]  handle handle as returned by ::tixiOpenDocument, ::tixiOpenDocumentRecursive or ::tixiOpenDocumentFromHTTP

  @param[in]  elementPath an XPath compliant path to an element in the document
                    specified by handle (see section \ref XPathExamples above).

  @param[out] number  content of the element specified by elementPath interpreted as a floating point number

  @return
    - SUCCESS if successfully retrieve the text content of a single element
    - INVALID_HANDLE if the handle is not valid, i.e.  does not or no longer exist
    - INVALID_XPATH if elementPath is not a well-formed XPath-expression
    - ELEMENT_NOT_FOUND if elementPath does not point to a node in the XML-document
    - ELEMENT_PATH_NOT_UNIQUE if elementPath resolves not to a single element but to a list of elements
 */
DLL_EXPORT ReturnCode tixiGetDoubleElement (const TixiDocumentHandle handle, const char *elementPath, double *number);


/**
  @brief Retrieve boolean content of an element.

  Returns the content of the element specified by elementPath in the
  document specified by handle as an integer number with the values 0=false and 1=true.
  elementPath must refer to exactly one element which has
  only a text node and zero or more attributes but no further
  children with text nodes. If an error occurs boolean is set to
  NULL. In the XML file, a boolean value could be defined with "1"/"0" or
  (case sensitive) "true"/"false".

  <b>Fortran syntax:</b>

  tixi_get_boolean_element( integer  handle, character*n element_path, integer boolean, integer error )

  @param[in]  handle handle as returned by ::tixiOpenDocument, ::tixiOpenDocumentRecursive or ::tixiOpenDocumentFromHTTP

  @param[in]  elementPath an XPath compliant path to an element in the document
                          specified by handle (see section \ref XPathExamples above).

  @param[out] boolean   content of the element specified by elementPath interpreted as a integer containing
                        boolean values.

  @return
    - SUCCESS if successfully retrieve the text content of a single element
    - INVALID_HANDLE if the handle is not valid, i.e.  does not or no longer exist
    - INVALID_XPATH if elementPath is not a well-formed XPath-expression
    - ELEMENT_NOT_FOUND if elementPath does not point to a node in the XML-document
    - ELEMENT_PATH_NOT_UNIQUE if elementPath resolves not to a single element but to a list of elements
 */
DLL_EXPORT ReturnCode tixiGetBooleanElement (const TixiDocumentHandle handle, const char *elementPath, int *boolean);


/**
  @brief Updates the text content of an element.

  Update the text content of the element specified by elementPath in the
  document specified by handle. elementPath must refer to exactly one
  element which has only a text node and zero or more attributes but
  no further children with text nodes.

  <b>Fortran syntax:</b>

  tixi_update_text_element( integer handle, character*n element_path,  character*n text, integer error )

  @param[in]  handle handle as returned by ::tixiOpenDocument, ::tixiOpenDocumentRecursive or ::tixiOpenDocumentFromHTTP

  @param[in]  elementPath an XPath compliant path to an element in the document
                    specified by handle (see section \ref XPathExamples above).

  @param[in]  text text content of the element to update the element specified by elementPath

  @return
    - SUCCESS if successfully retrieve the text content of a single element
    - INVALID_HANDLE if the handle is not valid, i.e.  does not or no longer exist
    - INVALID_XPATH if elementPath is not a well-formed XPath-expression
    - ELEMENT_NOT_FOUND if elementPath does not point to a node in the XML-document
    - ELEMENT_PATH_NOT_UNIQUE if elementPath resolves not to a single element but to a list of elements
    - FAILED if node is no textNode
   */
DLL_EXPORT ReturnCode tixiUpdateTextElement (const TixiDocumentHandle handle, const char *elementPath, const char *text);


/**
  @brief Updates the double content of an element.

  Update the double content of the element specified by elementPath in the
  document specified by handle. elementPath must refer to exactly one
  element which has only a text node and zero or more attributes but
  no further children with text nodes.

  <b>Fortran syntax:</b>

  tixi_update_double_element( integer handle, character*n element_path,  real number, character*n format, integer error )

  @param[in]  handle handle as returned by ::tixiOpenDocument, ::tixiOpenDocumentRecursive or ::tixiOpenDocumentFromHTTP

  @param[in]  elementPath an XPath compliant path to an element in the document
                          specified by handle (see section \ref XPathExamples above).

  @param[in]  number double content of the element to update the element specified by elementPath
  @param[in]  format format string used to convert number into a string.
                     The format string usage is identical to format strings in printf.
                     If format is NULL "%g" will be used to format the string.

  @return
    - SUCCESS if successfully retrieve the text content of a single element
    - INVALID_HANDLE if the handle is not valid, i.e.  does not or no longer exist
    - INVALID_XPATH if elementPath is not a well-formed XPath-expression
    - ELEMENT_NOT_FOUND if elementPath does not point to a node in the XML-document
    - ELEMENT_PATH_NOT_UNIQUE if elementPath resolves not to a single element but to a list of elements
    - FAILED if node is no textNode
 */
DLL_EXPORT ReturnCode tixiUpdateDoubleElement (const TixiDocumentHandle handle, const char *elementPath, double number, const char *format);

/**
  @brief Updates the double content of an element.

  Update the integer content of the element specified by elementPath in the
  document specified by handle. elementPath must refer to exactly one
  element which has only a text node and zero or more attributes but
  no further children with text nodes.

  <b>Fortran syntax:</b>

  tixi_update_integer_element( integer handle, character*n element_path,  integer number, character*n format, integer error )

  @param[in]  handle handle as returned by ::tixiOpenDocument, ::tixiOpenDocumentRecursive or ::tixiOpenDocumentFromHTTP

  @param[in]  elementPath an XPath compliant path to an element in the document
                          specified by handle (see section \ref XPathExamples above).

  @param[in]  number integer content of the element to update the element specified by elementPath
  @param[in]  format format string used to convert number into a string.
                     The format string usage is identical to format strings in printf.
                     If format is NULL "%g" will be used to format the string.

  @return
    - SUCCESS if successfully retrieve the text content of a single element
    - INVALID_HANDLE if the handle is not valid, i.e.  does not or no longer exist
    - INVALID_XPATH if elementPath is not a well-formed XPath-expression
    - ELEMENT_NOT_FOUND if elementPath does not point to a node in the XML-document
    - ELEMENT_PATH_NOT_UNIQUE if elementPath resolves not to a single element but to a list of elements
    - FAILED if node is no textNode
 */
DLL_EXPORT ReturnCode tixiUpdateIntegerElement (const TixiDocumentHandle handle, const char *elementPath, int number, const char *format);

/**
  @brief Updates the boolean content of an element.

  Update the boolean content of the element specified by elementPath in the
  document specified by handle. elementPath must refer to exactly one
  element which has only a text node and zero or more attributes but
  no further children with text nodes.

  <b>Fortran syntax:</b>

  tixi_update_boolean_element( integer handle, character*n element_path,  integer boolean, character*n format, integer error )

  @param[in]  handle handle as returned by ::tixiOpenDocument, ::tixiOpenDocumentRecursive or ::tixiOpenDocumentFromHTTP

  @param[in]  elementPath an XPath compliant path to an element in the document
                    specified by handle (see section \ref XPathExamples above).

  @param[in]  boolean boolean content of the element to update the element specified by elementPath. The value of boolean has to be "0" or "1".

  @return
    - SUCCESS if successfully retrieve the text content of a single element
    - INVALID_HANDLE if the handle is not valid, i.e.  does not or no longer exist
    - INVALID_XPATH if elementPath is not a well-formed XPath-expression
    - ELEMENT_NOT_FOUND if elementPath does not point to a node in the XML-document
    - ELEMENT_PATH_NOT_UNIQUE if elementPath resolves not to a single element but to a list of elements
    - FAILED if node is no textNode
 */
DLL_EXPORT ReturnCode tixiUpdateBooleanElement (const TixiDocumentHandle handle, const char *elementPath, int boolean);


/**
  @brief Creates an element holding text.

  Creates an element specified by the elementPath expression and
  insert text into the element. Elements with the same name can be
  added multiple times.

  <b>Fortran syntax:</b>

  tixi_add_text_element( integer  handle, character*n parent_path, character*n element_name, character*n text, integer error )

  @param[in]  handle file handle as returned by ::tixiCreateDocument

  @param[in]  parentPath an XPath compliant path to an element in the document
                         specified by handle (see section \ref XPathExamples above)
                         into which the new element is to be inserted. The parent
                         element has to exist already.

  @param[in]  elementName name of the element to be inserted into the parent element

  @param[in]  text text to be placed inside the element pointed to by elementPath. If
                   text is NULL an empty element will be created.

  @return
    - SUCCESS if successfully added the text element
    - INVALID_XML_NAME if elementName is not a valid XML-element name
    - INVALID_HANDLE if the handle is not valid
    - INVALID_XPATH if elementPath is not a well-formed XPath-expression
    - ELEMENT_PATH_NOT_UNIQUE if parentPath resolves not to a single element but to a list of elements
    - ELEMENT_NOT_FOUND if parentPath points to a non-existing element
    - ALREADY_SAVED if element should be added to an already saved document
*/
DLL_EXPORT ReturnCode tixiAddTextElement (const TixiDocumentHandle handle, const char *parentPath, const char *elementName, const char *text);


/**
  @brief Creates an element holding text within the given namespace.

  Creates an element specified by the elementPath expression and
  insert text into the element. Elements with the same name can be
  added multiple times.

  If the namespace does not yet exist in parent nodes, a xmlns attribute is added
  to the node. If the qualified name contains a prefix, the prefix is used as the
  namespace prefix also for all future childs nodes.

  <b>Fortran syntax:</b>

  tixi_add_text_element_ns( integer  handle, character*n parent_path, character*n qualified_name, character*n namespace_uri, character*n text, integer error )

  @param[in]  handle file handle as returned by ::tixiCreateDocument

  @param[in]  parentPath an XPath compliant path to an element in the document
                         specified by handle (see section \ref XPathExamples above)
                         into which the new element is to be inserted. The parent
                         element has to exist already.

  @param[in]  qualifiedName qualified name of the element to be inserted into the parent element.
                            This name might include a namespace prefix (e.g. "html:td")

  @param[in]  namespaceURI URI of the namespace that should be connected with the element.

  @param[in]  text text to be placed inside the element pointed to by elementPath. If
                   text is NULL an empty element will be created.

  @return
    - SUCCESS if successfully added the text element
    - INVALID_XML_NAME if elementName is not a valid XML-element name
    - INVALID_HANDLE if the handle is not valid
    - INVALID_XPATH if elementPath is not a well-formed XPath-expression
    - ELEMENT_PATH_NOT_UNIQUE if parentPath resolves not to a single element but to a list of elements
    - ELEMENT_NOT_FOUND if parentPath points to a non-existing element
    - INVALID_NAMESPACE_URI if namespaceURI is invalid (i.e. a null pointer)
    - ALREADY_SAVED if element should be added to an already saved document
*/
DLL_EXPORT ReturnCode tixiAddTextElementNS (const TixiDocumentHandle handle, const char *parentPath,
                                            const char *qualifiedName, const char *namespaceURI, const char *text);



/**
  @brief Creates an element holding text at a given index.

  Creates an element specified by the elementPath expression and
  insert text into the element. Elements with the same name can be
  added multiple times, so in this version of the function an index could be
  provides to specifiy a exact position.

  <b>Fortran syntax:</b>

  tixi_add_text_element_at_index( integer  handle, character*n parent_path, character*n element_name, character*n text, integer index, integer error )

  @param[in]  handle file handle as returned by ::tixiCreateDocument

  @param[in]  parentPath an XPath compliant path to an element in the document
                         specified by handle (see section \ref XPathExamples above)
                         into which the new element is to be inserted. The parent
                         element has to exist already.

  @param[in]  elementName name of the element to be inserted into the parent element

  @param[in]  text text to be placed inside the element pointed to by elementPath. If
                   text is NULL an empty element will be created.

  @param[in]  index the position index where the new node should be created.

  @return
    - SUCCESS if successfully added the text element
    - INVALID_XML_NAME if elementName is not a valid XML-element name
    - INVALID_HANDLE if the handle is not valid
    - INVALID_XPATH if elementPath is not a well-formed XPath-expression
    - ELEMENT_PATH_NOT_UNIQUE if parentPath resolves not to a single element but to a list of elements
    - ELEMENT_NOT_FOUND if parentPath points to a non-existing element
    - ALREADY_SAVED if element should be added to an already saved document
  */

DLL_EXPORT ReturnCode tixiAddTextElementAtIndex (const TixiDocumentHandle handle, const char *parentPath, const char *elementName, const char *text, int index);

/**
  @brief Creates an element holding text at a given index in the specified namespace.

  Creates an element specified by the elementPath expression and
  insert text into the element. Elements with the same name can be
  added multiple times, so in this version of the function an index could be
  provides to specifiy a exact position.

  If the namespace does not yet exist in parent nodes, a xmlns attribute is added
  to the node. If the qualified name contains a prefix, the prefix is used as the
  namespace prefix also for all future childs nodes.

  <b>Fortran syntax:</b>

  tixi_add_text_element_ns_at_index( integer  handle, character*n parent_path, character*n qualified_name, character*n namespace_uri, character*n text, integer index, integer error )

  @param[in]  handle file handle as returned by ::tixiCreateDocument

  @param[in]  parentPath an XPath compliant path to an element in the document
                         specified by handle (see section \ref XPathExamples above)
                         into which the new element is to be inserted. The parent
                         element has to exist already.

  @param[in]  qualifiedName qualified name of the element to be inserted into the parent element.
                            This name might include a namespace prefix (e.g. "html:td")

  @param[in]  namespaceURI URI of the namespace that should be connected with the element.

  @param[in]  text text to be placed inside the element pointed to by elementPath. If
                   text is NULL an empty element will be created.

  @param[in]  index the position index where the new node should be created.

  @return
    - SUCCESS if successfully added the text element
    - INVALID_XML_NAME if elementName is not a valid XML-element name
    - INVALID_HANDLE if the handle is not valid
    - INVALID_XPATH if elementPath is not a well-formed XPath-expression
    - ELEMENT_PATH_NOT_UNIQUE if parentPath resolves not to a single element but to a list of elements
    - ELEMENT_NOT_FOUND if parentPath points to a non-existing element
    - INVALID_NAMESPACE_URI if namespaceURI is invalid (i.e. a null pointer)
    - ALREADY_SAVED if element should be added to an already saved document
  */
DLL_EXPORT ReturnCode tixiAddTextElementNSAtIndex(const TixiDocumentHandle handle, const char *parentPath,
                                                  const char* qualifiedName, const char* namespaceURI, const char *text, int index);

/**
  @brief Creates an element and sets the value to "true" or "false".

  Creates an element specified by the elementPath expression and
  inserts one of the boolean values "true" or "false" into the element.
  Elements with the same name can be added multiple times.

  <b>Fortran syntax:</b>

  tixi_add_boolean_element( integer  handle, character*n parent_path, character*n element_name, integer boolean, integer error )

  @param[in]  handle file handle as returned by ::tixiCreateDocument

  @param[in]  parentPath an XPath compliant path to an element in the document
                         specified by handle (see section \ref XPathExamples above)
                         into which the new element is to be inserted. The parent
                         element has to exist already.

  @param[in]  elementName name of the element to be inserted into the parent element

  @param[in]  boolean boolean value to be placed inside the element pointed to by elementPath. The
                      value of boolean has to be "0" or "1".

  @return
    - SUCCESS if successfully added the text element
    - INVALID_XML_NAME if elementName is not a valid XML-element name
    - INVALID_HANDLE if the handle is not valid
    - INVALID_XPATH if elementPath is not a well-formed XPath-expression
    - ELEMENT_PATH_NOT_UNIQUE if parentPath resolves not to a single element but to a list of elements
    - ELEMENT_NOT_FOUND if parentPath points to a non-existing element
    - ALREADY_SAVED if element should be added to an already saved document
  */
DLL_EXPORT ReturnCode tixiAddBooleanElement (const TixiDocumentHandle handle, const char *parentPath, const char *elementName, int boolean);


/**
  @brief Creates an element in the given namesoace and sets the value to "true" or "false".

  Creates an element specified by the elementPath expression and
  inserts one of the boolean values "true" or "false" into the element.
  Elements with the same name can be added multiple times.

  If the namespace does not yet exist in parent nodes, a xmlns attribute is added
  to the node. If the qualified name contains a prefix, the prefix is used as the
  namespace prefix also for all future childs nodes.

  <b>Fortran syntax:</b>

  tixi_add_boolean_element_ns( integer  handle, character*n parent_path, character*n qualified_name, character*n namespace_uri, integer boolean, integer error )

  @param[in]  handle file handle as returned by ::tixiCreateDocument

  @param[in]  parentPath an XPath compliant path to an element in the document
                         specified by handle (see section \ref XPathExamples above)
                         into which the new element is to be inserted. The parent
                         element has to exist already.

  @param[in]  qualifiedName qualified name of the element to be inserted into the parent element.
                            This name might include a namespace prefix (e.g. "html:td")

  @param[in]  namespaceURI URI of the namespace that should be connected with the element.

  @param[in]  boolean boolean value to be placed inside the element pointed to by elementPath. The
                      value of boolean has to be "0" or "1".

  @return
    - SUCCESS if successfully added the text element
    - INVALID_XML_NAME if elementName is not a valid XML-element name
    - INVALID_HANDLE if the handle is not valid
    - INVALID_XPATH if elementPath is not a well-formed XPath-expression
    - ELEMENT_PATH_NOT_UNIQUE if parentPath resolves not to a single element but to a list of elements
    - ELEMENT_NOT_FOUND if parentPath points to a non-existing element
    - INVALID_NAMESPACE_URI if namespaceURI is invalid (i.e. a null pointer)
    - ALREADY_SAVED if element should be added to an already saved document
 */
DLL_EXPORT ReturnCode tixiAddBooleanElementNS (const TixiDocumentHandle handle, const char *parentPath,
                                               const char *qualifiedName, const char* namespaceURI, int boolean);


/**
  @brief Creates an element which holds a floating point number.

  Creates an element specified by the elementPath expression. Elements
  with the same name can be added multiple times.

  <b>Fortran syntax:</b>

  tixi_add_double_element( integer  handle, character*n parent_path, character*n element_name, real number, character*n format, integer error )

  @param[in]  handle file handle as returned by ::tixiOpenDocument or ::tixiCreateDocument

  @param[in]  parentPath an XPath compliant path to an element in the document
                         specified by handle (see section \ref XPathExamples above).

  @param[in]  elementName name of the element to be inserted into the parent element

  @param[in]  number floating point number to be placed inside the element pointed to by
                     elementPath. If number is NULL an empty element will be created.

  @param[in]  format format string used to convert number into a string.
                     The format string usage is identical to format strings in printf.
                     If format is NULL "%g" will be used to format the string.

  @return
    - SUCCESS if successfully retrieve the text content
    - INVALID_HANDLE if the handle is not valid, i.e.  does not or no longer exist
    - INVALID_XPATH if elementPath is not a well-formed XPath-expression
    - ELEMENT_NOT_FOUND if elementPath does not point to a node in the XML-document
    - ELEMENT_PATH_NOT_UNIQUE if elementPath resolves not to a single element but to a list of elements
 */
DLL_EXPORT ReturnCode tixiAddDoubleElement (const TixiDocumentHandle handle,
                                            const char *parentPath, const char *elementName,
                                            double number, const char *format);


/**
  @brief Creates an element which holds a floating point number in the given namespace.

  Creates an element specified by the elementPath expression. Elements
  with the same name can be added multiple times.

  If the namespace does not yet exist in parent nodes, a xmlns attribute is added
  to the node. If the qualified name contains a prefix, the prefix is used as the
  namespace prefix also for all future childs nodes.

  <b>Fortran syntax:</b>

  tixi_add_double_element_ns( integer  handle, character*n parent_path, character*n qualified_name, character*n namespace_uri, real number, character*n format, integer error )

  @param[in]  handle file handle as returned by ::tixiOpenDocument or ::tixiCreateDocument

  @param[in]  parentPath an XPath compliant path to an element in the document
                         specified by handle (see section \ref XPathExamples above).

  @param[in]  qualifiedName qualified name of the element to be inserted into the parent element.
                            This name might include a namespace prefix (e.g. "html:td")

  @param[in]  namespaceURI URI of the namespace that should be connected with the element.

  @param[in]  number floating point number to be placed inside the element pointed to by
                     elementPath. If number is NULL an empty element will be created.

  @param[in]  format format string used to convert number into a string.
                     The format string usage is identical to format strings in printf.
                     If format is NULL "%g" will be used to format the string.

  @return
    - SUCCESS if successfully retrieve the text content
    - INVALID_HANDLE if the handle is not valid, i.e.  does not or no longer exist
    - INVALID_XPATH if elementPath is not a well-formed XPath-expression
    - ELEMENT_NOT_FOUND if elementPath does not point to a node in the XML-document
    - INVALID_NAMESPACE_URI if namespaceURI is invalid (i.e. a null pointer)
    - ELEMENT_PATH_NOT_UNIQUE if elementPath resolves not to a single element but to a list of elements
 */
DLL_EXPORT ReturnCode tixiAddDoubleElementNS (const TixiDocumentHandle handle,
                                              const char *parentPath, const char *qualifiedName, const char* namespaceURI,
                                              double number, const char *format);

/**
  @brief Creates an element which holds an integer.

  Creates an element specified by the elementPath expression holding
  an integer number. Elements with the same name can be added multiple times.

  <b>Fortran syntax:</b>

  tixi_add_integer_element( integer  handle, character*n parent_path, character*n element_name, integer number, character*n format, integer error )

  @param[in]  handle file handle as returned by ::tixiOpenDocument or ::tixiCreateDocument

  @param[in]  parentPath an XPath compliant path to an element in the document
                         specified by handle (see section \ref XPathExamples above).

  @param[in]  elementName name of the element to be inserted into the parent element

  @param[in]  number integer number to be placed inside the element pointed to by
                     elementPath. If number is NULL an empty element will be created.

  @param[in]  format format string used to convert number into a string.
                     The format string usage is identical to format strings in printf.
                     If format is NULL "%g" will be used to format the string.

  @return
    - SUCCESS if successfully retrieve the text content
    - INVALID_HANDLE if the handle is not valid, i.e.  does not or no longer exist
    - INVALID_XPATH if elementPath is not a well-formed XPath-expression
    - ELEMENT_NOT_FOUND if elementPath does not point to a node in the XML-document
    - ELEMENT_PATH_NOT_UNIQUE if elementPath resolves not to a single element but to a list of elements
 */
DLL_EXPORT ReturnCode tixiAddIntegerElement (const TixiDocumentHandle handle,
                                             const char *parentPath, const char *elementName,
                                             int number, const char *format);


/**
  @brief Creates an element which holds an integer in the given namespace.

  Creates an element specified by the elementPath expression holding
  an integer number. Elements with the same name can be added multiple times.

  If the namespace does not yet exist in parent nodes, a xmlns attribute is added
  to the node. If the qualified name contains a prefix, the prefix is used as the
  namespace prefix also for all future childs nodes.

  <b>Fortran syntax:</b>

  tixi_add_integer_element_ns( integer  handle, character*n parent_path, character*n qualified_name, character*n namespace_uri, integer number, character*n format, integer error )

  @param[in]  handle file handle as returned by ::tixiOpenDocument or ::tixiCreateDocument

  @param[in]  parentPath an XPath compliant path to an element in the document
                         specified by handle (see section \ref XPathExamples above).

  @param[in]  qualifiedName qualified name of the element to be inserted into the parent element.
                            This name might include a namespace prefix (e.g. "html:td")

  @param[in]  namespaceURI URI of the namespace that should be connected with the element.

  @param[in]  number integer number to be placed inside the element pointed to by
                     elementPath. If number is NULL an empty element will be created.

  @param[in]  format format string used to convert number into a string.
                     The format string usage is identical to format strings in printf.
                     If format is NULL "%g" will be used to format the string.

  @return
    - SUCCESS if successfully retrieve the text content
    - INVALID_HANDLE if the handle is not valid, i.e.  does not or no longer exist
    - INVALID_XPATH if elementPath is not a well-formed XPath-expression
    - ELEMENT_NOT_FOUND if elementPath does not point to a node in the XML-document
    - INVALID_NAMESPACE_URI if namespaceURI is invalid (i.e. a null pointer)
    - ELEMENT_PATH_NOT_UNIQUE if elementPath resolves not to a single element but to a list of elements
 */
DLL_EXPORT ReturnCode tixiAddIntegerElementNS (const TixiDocumentHandle handle,
                                               const char *parentPath,
                                               const char *qualifiedName, const char *namespaceURI,
                                               int number, const char *format);


/**
  @brief Creates an element holding an vector.

  Creates an element specified by the elementPath expression and
  insert the vector elements into the element. Vector with the same name can be
  added multiple times.


  <b>Fortran syntax:</b>

  tixi_add_float_vector( integer handle, character*n parent_path, character*n element_name, real array, integer numElements, integer error )

  @cond
  #annotate in: 3A(4)#
  @endcond

  @param[in]  handle file handle as returned by ::tixiCreateDocument

  @param[in]  parentPath an XPath compliant path to an element in the document
                         specified by handle (see section \ref XPathExamples above)
                         into which the new element is to be inserted. The parent
                         element has to exist already.

  @param[in]  elementName name of the element to be inserted into the parent element

  @param[in]  vector The Vector to be placed inside the element pointed to by elementPath. If
                     Vector is NULL an empty element will be created.

  @param[in]  numElements the Number of vector-elements to be inserted in the new element.

  @param[in]  format format string used to convert number into a string.
                     The format string usage is identical to format strings in printf.
                     If format is NULL "%g" will be used to format the string.

  @return
    - SUCCESS if successfully added the text element
    - FAILED for internal errors
    - INVALID_XML_NAME if elementName is not a valid XML-element name
    - INVALID_HANDLE if the handle is not valid
    - INVALID_XPATH if elementPath is not a well-formed XPath-expression
    - ELEMENT_PATH_NOT_UNIQUE if parentPath resolves not to a single element but to a list of elements
    - ELEMENT_NOT_FOUND if parentPath points to a non-existing element
    - ALREADY_SAVED if element should be added to an already saved document
 */
DLL_EXPORT ReturnCode tixiAddFloatVector (const TixiDocumentHandle handle, const char *parentPath, const char *elementName, const double *vector, const int numElements, const char* format);


/**
  @brief Updates the data of a vector element.


  <b>Fortran syntax:</b>

  tixi_update_float_vector( integer handle, character*n path, real array, integer numElements, integer error )

  @cond
  #annotate in: 2A(3)#
  @endcond

  @param[in]  handle file handle as returned by ::tixiCreateDocument

  @param[in]  path   an XPath compliant path to an element in the document
                     specified by handle (see section \ref XPathExamples above).

  @param[in]  vector The new Vector data replacing the old data at path. If
                     Vector is NULL an empty element will be created.

  @param[in]  numElements the Number of vector-elements to be inserted in the new element.

  @param[in]  format format string used to convert number into a string.
                     The format string usage is identical to format strings in printf.
                     If format is NULL "%g" will be used to format the string.

  @return
    - SUCCESS if successfully added the text element
    - FAILED for internal errors
    - INVALID_XML_NAME if elementName is not a valid XML-element name
    - INVALID_HANDLE if the handle is not valid
    - INVALID_XPATH if path is not a well-formed XPath-expression
    - ELEMENT_PATH_NOT_UNIQUE if path resolves not to a single element but to a list of elements
    - ELEMENT_NOT_FOUND if path points to a non-existing element
    - ALREADY_SAVED if element should be added to an already saved document
 */
DLL_EXPORT ReturnCode tixiUpdateFloatVector (const TixiDocumentHandle handle, const char *path, const double *vector, const int numElements, const char* format);


/**
  @brief Creates an empty element.

  Creates an empty element specified by the elementPath expression
  Elements with the same name can be added multiple times.

  <b>Fortran syntax:</b>

  tixi_create_element( integer  handle, character*n parent_path, character*n element_name, integer error )

  @param[in]  handle file handle as returned by ::tixiCreateDocument

  @param[in]  parentPath an XPath compliant path to an element in the document
                         specified by handle (see section \ref XPathExamples above)
                         into which the new element is to be inserted. The parent
                         element has to exist already.

  @param[in]  elementName name of the element to be inserted into the parent element

  @return
    - SUCCESS if successfully added the text element
    - INVALID_XML_NAME if elementName is not a valid XML-element name
    - INVALID_HANDLE if the handle is not valid
    - INVALID_XPATH if elementPath is not a well-formed XPath-expression
    - ELEMENT_PATH_NOT_UNIQUE if parentPath resolves not to a single element but to a list of elements
    - ELEMENT_NOT_FOUND if parentPath points to a non-existing element
    - ALREADY_SAVED if element should be added to an already saved document
 */
DLL_EXPORT ReturnCode tixiCreateElement (const TixiDocumentHandle handle, const char *parentPath, const char *elementName);


/**
  @brief Creates an empty element in the specified namespace.

  Creates an empty element specified by the elementPath expression
  Elements with the same name can be added multiple times.

  If the namespace does not yet exist in parent nodes, a xmlns attribute is added
  to the node. If the qualified name contains a prefix, the prefix is used as the
  namespace prefix also for all future childs nodes.

  <b>Fortran syntax:</b>

  tixi_create_element_ns( integer  handle, character*n parent_path, character*n qualified_name, character*n namespace_uri, integer error )

  @param[in]  handle file handle as returned by ::tixiCreateDocument

  @param[in]  parentPath an XPath compliant path to an element in the document
                         specified by handle (see section \ref XPathExamples above)
                         into which the new element is to be inserted. The parent
                         element has to exist already.

  @param[in]  qualifiedName qualified name of the element to be inserted into the parent element.
                            This name might include a namespace prefix (e.g. "html:td")

  @param[in]  namespaceURI URI of the namespace that should be connected with the element.

  @return
    - SUCCESS if successfully added the text element
    - INVALID_XML_NAME if elementName is not a valid XML-element name
    - INVALID_HANDLE if the handle is not valid
    - INVALID_XPATH if elementPath is not a well-formed XPath-expression
    - ELEMENT_PATH_NOT_UNIQUE if parentPath resolves not to a single element but to a list of elements
    - ELEMENT_NOT_FOUND if parentPath points to a non-existing element
    - INVALID_NAMESPACE_URI if namespaceURI is invalid (i.e. a null pointer)
    - ALREADY_SAVED if element should be added to an already saved document
 */
DLL_EXPORT ReturnCode tixiCreateElementNS (const TixiDocumentHandle handle, const char *parentPath,
                                           const char *qualifiedName, const char* namespaceURI);


/**
  @brief Creates an empty element at a given index.

  Creates an empty element specified by the elementPath expression. In this function you need to provide an index > 0
        on which position the new element should be created.
  Elements with the same name can be added multiple times.

  <b>Fortran syntax:</b>

  tixi_create_element_at_index( integer  handle, character*n parent_path, character*n element_name, integer index, integer error )

  @param[in]  handle file handle as returned by ::tixiCreateDocument

  @param[in]  parentPath an XPath compliant path to an element in the document
                         specified by handle (see section \ref XPathExamples above)
                         into which the new element is to be inserted. The parent
                         element has to exist already.

  @param[in]  elementName name of the element to be inserted into the parent element

  @param[in]  index       position of the new created element

  @return
    - SUCCESS if successfully added the text element
    - INVALID_XML_NAME if elementName is not a valid XML-element name
    - INVALID_HANDLE if the handle is not valid
    - INVALID_XPATH if elementPath is not a well-formed XPath-expression
    - ELEMENT_PATH_NOT_UNIQUE if parentPath resolves not to a single element but to a list of elements
    - ELEMENT_NOT_FOUND if parentPath points to a non-existing element
    - ALREADY_SAVED if element should be added to an already saved document
 */
DLL_EXPORT ReturnCode tixiCreateElementAtIndex (const TixiDocumentHandle handle, const char *parentPath, const char *elementName, int index);


/**
  @brief Creates an empty element at a given index in the specified namespace.

  Creates an empty element specified by the elementPath expression. In this function you need to provide an index > 0
        on which position the new element should be created.
  Elements with the same name can be added multiple times.

  If the namespace does not yet exist in parent nodes, a xmlns attribute is added
  to the node. If the qualified name contains a prefix, the prefix is used as the
  namespace prefix also for all future childs nodes.

  <b>Fortran syntax:</b>

  tixi_create_element_at_index_ns( integer  handle, character*n parent_path, character*n qualified_name, character*n namespace_uri, integer index, integer error )

  @param[in]  handle file handle as returned by ::tixiCreateDocument

  @param[in]  parentPath an XPath compliant path to an element in the document
                         specified by handle (see section \ref XPathExamples above)
                         into which the new element is to be inserted. The parent
                         element has to exist already.

  @param[in]  qualifiedName qualified name of the element to be inserted into the parent element.
                            This name might include a namespace prefix (e.g. "html:td")

  @param[in]  namespaceURI URI of the namespace that should be connected with the element.

  @param[in]  index       position of the new created element

  @return
    - SUCCESS if successfully added the text element
    - INVALID_XML_NAME if elementName is not a valid XML-element name
    - INVALID_HANDLE if the handle is not valid
    - INVALID_XPATH if elementPath is not a well-formed XPath-expression
    - ELEMENT_PATH_NOT_UNIQUE if parentPath resolves not to a single element but to a list of elements
    - ELEMENT_NOT_FOUND if parentPath points to a non-existing element
    - INVALID_NAMESPACE_URI if namespaceURI is invalid (i.e. a null pointer)
    - ALREADY_SAVED if element should be added to an already saved document
 */
DLL_EXPORT ReturnCode tixiCreateElementNSAtIndex (const TixiDocumentHandle handle, const char *parentPath,
                                                  const char *qualifiedName, int index, const char* namespaceURI);


/**
  @brief Renames an element

  This function renames an element from the document.

  <b>Fortran syntax:</b>

  tixi_rename_element( integer handle, character*n parent_path, character*n old_name, character*n new_name )

  @param[in] handle file handle as returned by ::tixiCreateDocument
  @param[in] parentPath an XPath complian path to the elements parent
                         of the element that shall be renamed
  @param[in] oldName the element that shall be renamed
  @param[in] newName the new name for the element
  @return
    - SUCCESS if the element was renamed successfully
    - INVALID_HANDLE if the handle is not valid, i.e. does not or no longer exist
    - INVALD_XPATH if the parentPath is not a well-formed XPath-expression
    - ELEMENT_NOT_FOUND if there does not exist an element with name "oldName" unter
                        the parentPath
 */
DLL_EXPORT ReturnCode tixiRenameElement(const TixiDocumentHandle handle,
                                        const char* parentPath,
                                        const char* oldName,
                                        const char* newName);

/**
  @brief Removes an element.

  Removes an element from the document. It is not an error to remove
  a non existing element.

  <b>Fortran syntax:</b>

  tixi_remove_element( integer  handle, character*n element_path, integer error )

  @param[in]  handle file handle as returned by ::tixiCreateDocument

  @param[in]  elementPath an XPath compliant path to an element in the
                     document specified by handle (see section \ref XPathExamples above).

  @return
    - SUCCESS if successfully removed the element
    - INVALID_HANDLE if the handle is not valid, i.e.  does not or no longer exist
    - INVALID_XPATH if elementPath is not a well-formed XPath-expression
    - ELEMENT_NOT_FOUND if elementPath does not point to a node in the XML-document
    - ELEMENT_PATH_NOT_UNIQUE if elementPath resolves not to a single element but
                              to a list of elements
 */
DLL_EXPORT ReturnCode tixiRemoveElement (const TixiDocumentHandle handle, const char *elementPath);


/**
  @brief Returns the number of child elements beneath a given path.

  <b>Fortran syntax:</b>

  tixi_get_node_type( integer handle, character*n element_path, integer error )

  @param[in]  handle   handle as returned by ::tixiCreateDocument, ::tixiOpenDocumentRecursive or ::tixiOpenDocumentFromHTTP
  @param[in]  nodePath an XPath compliant path to an element or node in the document
                       specified by handle (see section \ref XPathExamples above).
  @param[out] nodeType String containing the type of the node. The nodes types are named according to
                       the xml standard, defined on http://www.w3schools.com/dom/dom_nodetype.asp under
                       the section NodeTypes - Named Constants.

  @return
    - SUCCESS if the type is obtained
    - INVALID_HANDLE if the handle is not valid, i.e.  does not or no longer exist
    - INVALID_XPATH if elementPath is not a well-formed XPath-expression
    - ELEMENT_NOT_FOUND if elementPath does not point to a node in the XML-document
    - ELEMENT_PATH_NOT_UNIQUE if elementPath resolves not to a single element but
                             to a list of elements
 */
DLL_EXPORT ReturnCode tixiGetNodeType (const TixiDocumentHandle handle,
                                       const char *nodePath, char **nodeType);


/**
  @brief Returns the number of children elements with the same name.

  <b>Fortran syntax:</b>

  tixi_get_named_children_count( integer handle, character*n element_path, character*n child_name, int* count, integer error )

  @param[in]  handle handle as returned by ::tixiCreateDocument, ::tixiOpenDocumentRecursive or ::tixiOpenDocumentFromHTTP
  @param[in]  elementPath the path to an element in the document
                          specified by handle (see section \ref XPathExamples above).
  @param[in]  childName name of children to be counted
  @param[out] count  number of children with name childName.
                     0 is returned if either the element specified by elementPath has no
                     children at all or has no children with name childName.

  @return
    - SUCCESS if a count is computed
    - INVALID_HANDLE if the handle is not valid, i.e.  does not or no longer exist
    - INVALID_XPATH if elementPath is not a well-formed XPath-expression
    - ELEMENT_NOT_FOUND if elementPath does not point to a node in the XML-document
    - ELEMENT_PATH_NOT_UNIQUE if elementPath resolves not to a single element but
                              to a list of elements
    - NO_CHILD_NAME if childName is NULL
 */
DLL_EXPORT ReturnCode tixiGetNamedChildrenCount (const TixiDocumentHandle handle,
                                                 const char *elementPath, const char *childName,
                                                 int *count);

/**
  @brief Returns the name of a child node beneath a given path.

  <b>Fortran syntax:</b>

  tixi_get_child_node_name( integer handle, character*n element_path, int* index, character*n child_name_array, integer error )

  @param[in]  handle handle as returned by ::tixiCreateDocument, ::tixiOpenDocumentRecursive or ::tixiOpenDocumentFromHTTP
  @param[in]  parentElementPath the path to the parent element in the document
                                specified by handle (see section \ref XPathExamples above).
  @param[in]  index number index of the child-element of the given path.
  @param[out] name  String containing the name of the child node. If the node is not a normal node, the name variable will contain:
                     - \#text - in case of a text node
                     - \#comment - in case of a comment node
                     - \#cdata-section - in case of a CDATA section node

  @return
    - SUCCESS if a count is computed
    - INVALID_HANDLE if the handle is not valid, i.e.  does not or no longer exist
    - INVALID_XPATH if elementPath is not a well-formed XPath-expression
    - ELEMENT_NOT_FOUND if elementPath does not point to a node in the XML-document
    - ELEMENT_PATH_NOT_UNIQUE if elementPath resolves not to a single element but
                              to a list of elements
*/
DLL_EXPORT ReturnCode tixiGetChildNodeName (const TixiDocumentHandle handle,
                                            const char *parentElementPath, int index, char **name);

/**
  @brief Returns the number of child elements beneath a given path.

  <b>Fortran syntax:</b>

  tixi_get_number_of_childs( integer handle, character*n element_path, int* nchilds, integer error )

  @param[in]  handle handle as returned by ::tixiCreateDocument, ::tixiOpenDocumentRecursive or ::tixiOpenDocumentFromHTTP
  @param[in]  elementPath an XPath compliant path to an element in the document
                          specified by handle (see section \ref XPathExamples above).
  @param[out] nChilds Number of child elements beneath the given elementPath.

  @return
    - SUCCESS if a count is computed
    - INVALID_HANDLE if the handle is not valid, i.e.  does not or no longer exist
    - INVALID_XPATH if elementPath is not a well-formed XPath-expression
    - ELEMENT_NOT_FOUND if elementPath does not point to a node in the XML-document
    - ELEMENT_PATH_NOT_UNIQUE if elementPath resolves not to a single element but
                              to a list of elements
*/
DLL_EXPORT ReturnCode tixiGetNumberOfChilds(const TixiDocumentHandle handle, const char *elementPath, int* nChilds);

/**
  @brief Swaps to xml elements with each other

  The function does not allow to swap a parent element with a (sub) child.

  @param[in] handle handle as returned by ::tixiCreateDocument, ::tixiOpenDocumentRecursive or ::tixiOpenDocumentFromHTTP.
  @param[in] element1Path an XPath compliant path to the first element.
  @param[in] element2Path an XPath compliant path to the second element.

  @return
    - SUCCESS if swap was successful
    - FAILED if one element is parent of the other element.
    - INVALID_HANDLE if the handle is not valid, i.e.  does not or no longer exist
    - INVALID_XPATH if element1Path or element2Path is not a well-formed XPath-expression
    - ELEMENT_NOT_FOUND if element1Path or element2Path does not point to a node in the XML-document
    - ELEMENT_PATH_NOT_UNIQUE if element1Path or element2Path resolves not to a single element but
                              to a list of elements
 */
DLL_EXPORT ReturnCode tixiSwapElements(const TixiDocumentHandle handle, const char* element1Path, const char* element2Path);

/*@}*/

/**
  \defgroup Attributes Attribute Handling Functions

  Functions to get the content of an element attribute as a string or a number,
  functions to create and manipulate attributes, and a function to remove attributes
  are described in this section.
 */
/*@{*/

/**
  @brief Retrieves value of an element's attribute as a string.

  Returns the value of an attribute specified by attributeName of the
  element, specified by elementPath, in the document specified by
  handle. On successful return the memory used for value is allocated
  internally and must not be released by the user. The memory is
  deallocated when the document referred to by handle is closed.

  <b>Fortran syntax:</b>

  tixi_get_text_attribute( integer  handle, character*n element_path, character*n attribute_name, character*n text, integer error )

  @param[in]  handle handle as returned by ::tixiOpenDocument or ::tixiCreateDocument

  @param[in]  elementPath an XPath compliant path to an element in the document
                          specified by handle (see section \ref XPathExamples above).

  @param[in]  attributeName name of the attribute to be added to the element. The name
                            can also consist of a namespace prefix + ":" + the attribute
                            name.

  @param[out] text value of the specified attribute as a string

  @return
    - SUCCESS if successfully retrieve the text content of a single element
    - INVALID_HANDLE if the handle is not valid, i.e.  does not or no longer exist
    - INVALID_XPATH if elementPath is not a well-formed XPath-expression
    - ATTRIBUTE_NOT_FOUND if the element has no attribute attributeName
    - ELEMENT_NOT_FOUND if elementPath does not point to a node in the XML-document
    - ELEMENT_PATH_NOT_UNIQUE if elementPath resolves not to a single element but to a list of elements
    - INVALID_NAMESPACE_PREFIX if the prefix in attributeName does not match to a namespace
 */
DLL_EXPORT ReturnCode tixiGetTextAttribute (const TixiDocumentHandle handle,
                                            const char *elementPath, const char *attributeName,
                                            char **text);

/**
  @brief Retrieves value of an element's attribute as an integer.

  Returns the value of an attribute specified by attributeName of the
  element, specified by elementPath, in the document specified by
  handle. On successful return the memory used for value is allocated
  internally and must not be released by the user. The memory is
  deallocated when the document referred to by handle is closed.

  <b>Fortran syntax:</b>

  tixi_get_integer_attribute( integer  handle, character*n element_path, character*n attribute_name, integer *number, integer error )

  @param[in]  handle handle as returned by ::tixiOpenDocument or ::tixiCreateDocument

  @param[in]  elementPath an XPath compliant path to an element in the document
                          specified by handle (see section \ref XPathExamples above).

  @param[in]  attributeName name of the attribute to be added to the element. The name
                            can also consist of a namespace prefix + ":" + the attribute
                            name.

  @param[out] number  value of the specified attribute as an integer value

  @return
    - SUCCESS if successfully retrieve the text content of a single element
    - INVALID_HANDLE if the handle is not valid, i.e.  does not or no longer exist
    - INVALID_XPATH if elementPath is not a well-formed XPath-expression
    - ATTRIBUTE_NOT_FOUND if the element has no attribute attributeName
    - ELEMENT_NOT_FOUND if elementPath does not point to a node in the XML-document
    - ELEMENT_PATH_NOT_UNIQUE if elementPath resolves not to a single element but to a list of elements
    - INVALID_NAMESPACE_PREFIX if the prefix in attributeName does not match to a namespace
 */
DLL_EXPORT ReturnCode tixiGetIntegerAttribute (const TixiDocumentHandle handle,
                                               const char *elementPath, const char *attributeName,
                                               int *number);

/**
  @brief Retrieves value of an element's attribute as an boolean.

  Returns the value of an attribute specified by attributeName of the
  element, specified by elementPath, in the document specified by
  handle. On successful return the memory used for value is allocated
  internally and must not be released by the user. The memory is
  deallocated when the document referred to by handle is closed.

  <b>Fortran syntax:</b>

  tixi_get_boolean_attribute( integer  handle, character*n element_path, character*n attribute_name, integer boolean, integer error )

  @param[in]  handle handle as returned by ::tixiOpenDocument or ::tixiCreateDocument

  @param[in]  elementPath an XPath compliant path to an element in the document
                          specified by handle (see section \ref XPathExamples above).

  @param[in]  attributeName name of the attribute to be added to the element. The name
                            can also consist of a namespace prefix + ":" + the attribute
                            name.

  @param[out] boolean  value of the specified attribute as an boolean value

  @return
    - SUCCESS if successfully retrieve the text content of a single element
    - INVALID_HANDLE if the handle is not valid, i.e.  does not or no longer exist
    - INVALID_XPATH if elementPath is not a well-formed XPath-expression
    - ATTRIBUTE_NOT_FOUND if the element has no attribute attributeName
    - ELEMENT_NOT_FOUND if elementPath does not point to a node in the XML-document
    - ELEMENT_PATH_NOT_UNIQUE if elementPath resolves not to a single element but to a list of elements
    - INVALID_NAMESPACE_PREFIX if the prefix in attributeName does not match to a namespace
 */
DLL_EXPORT ReturnCode tixiGetBooleanAttribute (const TixiDocumentHandle handle,
                                               const char *elementPath, const char *attributeName,
                                               int *boolean);

/**
  @brief Retrieves value of an element's attribute as a floating point number.

  Returns the value of an attribute specified by attributeName of the
  element, specified by elementPath, in the document specified by
  handle. On successful return the memory used for value is allocated
  internally and must not be released by the user. The memory is
  deallocated when the document referred to by handle is closed.

  <b>Fortran syntax:</b>

  tixi_get_double_attribute( integer  handle, character*n element_path, character*n attribute_name, real *number, integer error )

  @param[in]  handle handle as returned by ::tixiOpenDocument or ::tixiCreateDocument

  @param[in]  elementPath an XPath compliant path to an element in the document
                          specified by handle (see section \ref XPathExamples above).

  @param[in]  attributeName name of the attribute to be added to the element. The name
                            can also consist of a namespace prefix + ":" + the attribute
                            name.

  @param[out] number value of the specified attribute as a floating point value

  @return
    - SUCCESS if successfully retrieve the text content of a single element
    - INVALID_HANDLE if the handle is not valid, i.e.  does not or no longer exist
    - INVALID_XPATH if elementPath is not a well-formed XPath-expression
    - ATTRIBUTE_NOT_FOUND if the element has no attribute attributeName
    - ELEMENT_NOT_FOUND if elementPath does not point to a node in the XML-document
    - ELEMENT_PATH_NOT_UNIQUE if elementPath resolves not to a single element but to a list of elements
    - INVALID_NAMESPACE_PREFIX if the prefix in attributeName does not match to a namespace
 */
DLL_EXPORT ReturnCode tixiGetDoubleAttribute (const TixiDocumentHandle handle,
                                              const char *elementPath, const char *attributeName,
                                              double *number);




/**
  @brief Adds an attribute with a string value to an element.

  Adds an attribute with name attributeName and value attributeValue
  to an element specified by the elementPath expression. If the
  attribute already exists its previous value is replaced by text.

  <b>Fortran syntax:</b>

  tixi_add_text_attribute( integer  handle, character*n element_path, character*n attribute_name, character*n attribute_value, integer error )

  @param[in]  handle file handle as returned by ::tixiOpenDocument or ::tixiCreateDocument

  @param[in]  elementPath an XPath compliant path to an element in the document
                          specified by handle (see section \ref XPathExamples above).

  @param[in]  attributeName name of the attribute to be added to the element. The name
                            can also consist of a namespace prefix + ":" + the attribute
                            name.

  @param[in]  attributeValue text to assigned to the attribute. If attributeValue is
                             NULL the empty string will be assigned to the attribute.

  @return
    - SUCCESS if successfully retrieve the text content
    - INVALID_HANDLE if the handle is not valid, i.e.  does not or no longer exist
    - INVALID_XPATH if elementPath is not a well-formed XPath-expression
    - ELEMENT_NOT_FOUND if elementPath does not point to a node in the XML-document
    - NO_ATTRIBUTE_NAME if attributeName is NULL
    - ELEMENT_PATH_NOT_UNIQUE if elementPath resolves not to a single element but
                              to a list of elements
    - ALREADY_SAVED if element should be added to an already saved document
    - INVALID_XML_NAME if attributeName is not a valid XML-element name
    - INVALID_NAMESPACE_PREFIX if the prefix in attributeName does not match to a namespace
 */
DLL_EXPORT ReturnCode tixiAddTextAttribute (const TixiDocumentHandle handle,
                                            const char *elementPath, const char *attributeName,
                                            const char *attributeValue);


/**
  @brief Adds an attribute with a floating point number value to an element.

  Adds an attribute with name attributeName and a floating point
  number value to an element specified by the elementPath
  expression. If the attribute already exists its previous value is
  replaced.

  <b>Fortran syntax:</b>

  tixi_add_double_attribute( integer  handle, character*n element_path, character*n attribute_name, real number, character*n format, integer error )

  @param[in]  handle file handle as returned by ::tixiOpenDocument or ::tixiCreateDocument

  @param[in]  elementPath an XPath compliant path to an element in the document
                          specified by handle (see section \ref XPathExamples above).

  @param[in]  attributeName name of the attribute to be added to the element. The name
                            can also consist of a namespace prefix + ":" + the attribute
                            name.

  @param[in]  number floating point value to be assigned to the attribute. If
                     number is NULL an error is return and the attribute
                     is not created.

  @param[in]  format format string used to convert number into a string.
                     The format string usage is identical to format strings in printf.
                     If format is NULL "%g" will be used to format the string.

  @return
    - SUCCESS if successfully retrieve the text content
    - INVALID_HANDLE if the handle is not valid, i.e.  does not or no longer exist
    - INVALID_XPATH if elementPath is not a well-formed XPath-expression
    - ELEMENT_NOT_FOUND if elementPath does not point to a node in the XML-document
    - NO_NUMBER if number is NULL
    - NO_ATTRIBUTE_NAME if attributeName is NULL
    - ELEMENT_PATH_NOT_UNIQUE if elementPath resolves not to a single element but
                              to a list of elements
    - INVALID_NAMESPACE_PREFIX if the prefix in attributeName does not match to a namespace
 */
DLL_EXPORT ReturnCode tixiAddDoubleAttribute (const TixiDocumentHandle handle,
                                              const char *elementPath, const char *attributeName,
                                              double number, const char *format);


/**
  @brief Adds an attribute with an integer number value to an element.

  Adds an attribute with name attributeName and a integer number
  value to an element specified by the elementPath expression. If the
  attribute already exists its previous value is replaced.

  <b>Fortran syntax:</b>

  tixi_add_integer_attribute( integer  handle, character*n element_path, character*n attribute_name, integer number, character*n format, integer error )

  @param[in]  handle file handle as returned by ::tixiOpenDocument or ::tixiCreateDocument

  @param[in]  elementPath an XPath compliant path to an element in the
                          document specified by handle (see section \ref XPathExamples above).

  @param[in]  attributeName name of the attribute to be added to the element. The name
                            can also consist of a namespace prefix + ":" + the attribute
                            name.

  @param[in]  number integer value to be assigned to the attribute. If
                     number is NULL an error is return and the attribute
                     is not created.

  @param[in]  format format string used to convert number into a string.
                     The format string usage is identical to format strings in printf.
                     If format is NULL "%g" will be used to format the string.

  @return
    - SUCCESS if successfully retrieve the text content
    - INVALID_HANDLE if the handle is not valid, i.e.  does not or no longer exist
    - INVALID_XPATH if elementPath is not a well-formed XPath-expression
    - ELEMENT_NOT_FOUND if elementPath does not point to a node in the XML-document
    - NO_NUMBER if number is NULL
    - NO_ATTRIBUTE_NAME if attributeName is NULL
    - ELEMENT_PATH_NOT_UNIQUE if elementPath resolves not to a single element but
                              to a list of elements
    - INVALID_NAMESPACE_PREFIX if the prefix in attributeName does not match to a namespace
 */
DLL_EXPORT ReturnCode tixiAddIntegerAttribute (const TixiDocumentHandle handle,
                                               const char *elementPath, const char *attributeName,
                                               int number, const char *format);

/**
  @brief Removes an attribute

  Removes an attribute from an element. It is not an error to remove
  an non existing attribute.

  <b>Fortran syntax:</b>

  tixi_remove_attribute( integer handle, character*n element_path, character*n attribute_name, integer error )

  @param[in]  handle file handle as returned by ::tixiOpenDocument or ::tixiCreateDocument

  @param[in]  elementPath an XPath compliant path to an element in the
                          document specified by handle (see section \ref XPathExamples above).

  @param[in]  attributeName name of the attribute to be added to the element

  @return
    - SUCCESS if successfully removed the attribute
    - INVALID_HANDLE if the handle is not valid, i.e.  does not or no longer exist
    - INVALID_XPATH if elementPath is not a well-formed XPath-expression
    - ELEMENT_NOT_FOUND if elementPath does not point to a node in the XML-document
    - NO_ATTRIBUTE_NAME if attributeName is NULL
    - ELEMENT_PATH_NOT_UNIQUE if elementPath resolves not to a single element but
                              to a list of elements
    - ATTRIBUTE_NOT_FOUND, if the attribute could not be removed (i.e. if it does not exist)
    - INVALID_NAMESPACE_PREFIX, if the prefix in attributeName is invalid.
 */
DLL_EXPORT ReturnCode tixiRemoveAttribute (const TixiDocumentHandle handle,
                                           const char *elementPath, const char *attributeName);


/**
  @brief Returns the number of attributes  of a given node.

  <b>Fortran syntax:</b>

  tixi_get_number_of_attributes( integer handle, character*n element_path, int* nattr, integer error )

  @param[in]  handle handle as returned by ::tixiCreateDocument, ::tixiOpenDocumentRecursive or ::tixiOpenDocumentFromHTTP
  @param[in]  elementPath an XPath compliant path to an element in the document
                          specified by handle (see section \ref XPathExamples above).
  @param[out] nAttributes Number of attributes of a given node.

  @return
    - SUCCESS if a count is computed
    - INVALID_HANDLE if the handle is not valid, i.e.  does not or no longer exist
    - INVALID_XPATH if elementPath is not a well-formed XPath-expression
    - ELEMENT_NOT_FOUND if elementPath does not point to a node in the XML-document
    - ELEMENT_PATH_NOT_UNIQUE if elementPath resolves not to a single element but
                              to a list of elements
 */
DLL_EXPORT ReturnCode tixiGetNumberOfAttributes(const TixiDocumentHandle handle, const char *elementPath, int* nAttributes);


/**
  @brief Returns the name of an attribute beneath a given path.

  <b>Fortran syntax:</b>

  tixi_get_attribute_name( integer handle, character*n element_path, int* index, character*n attr_name_array, integer error )

  @param[in]  handle handle as returned by ::tixiCreateDocument, ::tixiOpenDocumentRecursive or ::tixiOpenDocumentFromHTTP
  @param[in]  elementPath an XPath compliant path to an element in the document
                          specified by handle (see section \ref XPathExamples above).
  @param[in]  attrIndex number index of the attribute of the given path (counting from 1...tixiGetNumberOfAttributes)
  @param[out] attrName  String containing the attribute name.

  @return
    - SUCCESS if a count is computed
    - INVALID_HANDLE if the handle is not valid, i.e.  does not or no longer exist
    - INVALID_XPATH if elementPath is not a well-formed XPath-expression
    - ELEMENT_NOT_FOUND if elementPath does not point to a node in the XML-document
    - ELEMENT_PATH_NOT_UNIQUE if elementPath resolves not to a single element but
                              to a list of elements
 */
DLL_EXPORT ReturnCode tixiGetAttributeName(const TixiDocumentHandle handle, const char *elementPath, int attrIndex, char** attrName);

/*@}*/

/**
  \defgroup Namespaces Namespace Support Functions

  These functions are used, to register xml namespaces to the parser or add namespaces to
  existing elements. For the following explanations, consider the following xml file

  @verbatim
  <root>
    <h:table xmlns:h="http://www.w3.org/TR/html4/">
      <h:tr>
        <h:td>Apples</h:td>
        <h:td>Bananas</h:td>
      </h:tr>
    </h:table>
    <aircraft xmlns="http://www.dlr.de/cpacs">
      <modelname>D150</modelname>
    </aircraft>
  </root>
  @endverbatim

  <H2>Reading elements with namespaces</H2>

  This file defines two namespaces - "http://www.w3.org/TR/html4/" and "http://www.dlr.de/cpacs".
  A query for "/root/h:table" or "/root/aircraft" will result in an error, as no prefix was defined
  for both namespaces. The prefix inside the xml file exists only in the file, but not in the parser.
  To successfully query these paths, the namespaces must be registered with some (arbitrary) prefix using
  ::tixiRegisterNamespace or ::tixiRegisterNamespacesFromDocument.

  After calling ::tixiRegisterNamespace(handle, "http://www.w3.org/TR/html4/", "html") the query for
  "/root/html:table" is now valid. After calling ::tixiRegisterNamespace(handle, "http://www.dlr.de/cpacs", "cpacs")
  the query for "/root/cpacs:aircraft" becomes valid.

  <H2>Writing elements with namespaces</H2>

  To write elements with namespaces, the following functions are available:
   - ::tixiCreateElementNS
   - ::tixiCreateElementNSAtIndex
   - ::tixiAddBooleanElementNS
   - ::tixiAddDoubleElementNS
   - ::tixiAddIntegerElementNS
   - ::tixiAddTextElementNS
   - ::tixiAddTextElementNSAtIndex

  To reproduce the upper xml file, use the following tixi code:

  @verbatim
  tixiCreateDocument("root", &handle);

  // create table element in prefixed html namespace
  tixiCreateElementNS(handle, "/root", "h:table", "http://www.w3.org/TR/html4/");
  tixiRegisterNamespace(handle, "http://www.w3.org/TR/html4/", "h");

  tixiCreateElementNS(handle, "/root/h:table", "tr", "http://www.w3.org/TR/html4/");
  tixiAddTextElementNS(handle, "/root/h:table/h:tr", "td", "http://www.w3.org/TR/html4/", "Apples");
  tixiAddTextElementNS(handle, "/root/h:table/h:tr", "td", "http://www.w3.org/TR/html4/", "Bananas"):

  // create aircraft in default namespace (no prefix!)
  tixiCreateElementNS(handle, "/root", "aircraft", "http://www.dlr.de/cpacs");
  tixiRegisterNamespace(handle, "http://www.dlr.de/cpacs", "c");
  tixiAddTextElementNS(handle, "/root/c:aircraft", "modelname", "http://www.dlr.de/cpacs", "D150");
  @endverbatim
 */
/*@{*/


/**
  @brief Registers the given namespace and its prefix.

  When dealing with xml namespaces, elements can only be accessed via xPath
  when their namespace was registered with a prefix.

  To automatically register all namespaces and prefixes in the current
  document, use ::tixiRegisterNamespacesFromDocument.

  <b>Fortran syntax:</b>

  tixi_register_namespace( integer handle, character*n namespace_uri, character*n prefix, integer error )

  @param[in]  handle handle as returned by ::tixiCreateDocument, ::tixiOpenDocumentRecursive or ::tixiOpenDocumentFromHTTP
  @param[in]  namespaceURI The URI for the namespace (e.g. "http://www.w3.org/TR/html4/")
  @param[in]  prefix The desired prefix of the namespace (e.g. "html")

  @return
    - SUCCESS if the namespace could be registered successfully
    - INVALID_HANDLE if the handle is not valid, i.e.  does not or no longer exist
    - FAILED In case of an error
 */
DLL_EXPORT ReturnCode tixiRegisterNamespace(const TixiDocumentHandle handle, const char* namespaceURI, const char* prefix);


/**
 @brief Registers all prefixed namespaces of the xml document to the parser.

 Default (non-prefixed) namespaces must be still registered using ::tixiRegisterNamespace manually
 to be accessible via xPath.

 In the example above, the namespace "http://www.w3.org/TR/html4/" would be registered with
 the prefix "h" and a query to "/root/h:table" becomes successful.

  <b>Fortran syntax:</b>

  tixi_register_namespaces_from_document( integer handle, integer error )

  @param[in]  handle handle as returned by ::tixiCreateDocument, ::tixiOpenDocumentRecursive or ::tixiOpenDocumentFromHTTP

  @return
    - SUCCESS if the namespace could be registered successfully
    - INVALID_HANDLE if the handle is not valid, i.e.  does not or no longer exist
    - FAILED In case of an error.
 */
DLL_EXPORT ReturnCode tixiRegisterNamespacesFromDocument(const TixiDocumentHandle handle);


/**
  @brief This function sets the namespace for the specified element.

  This function can be used, after an element was created or read
  from file to set the namespace of the element.
  To set a default namespace for this element, add NULL as the prefix.

  <b>Fortran syntax:</b>

  tixi_set_element_namespace( integer handle, character*n element_path, character*n namespace_uri, character*n prefix, integer error )

  @return
    - SUCCESS if the namespace could be set successfully
    - INVALID_HANDLE if the handle is not valid, i.e.  does not or no longer exist
    - INVALID_NAMESPACE_URI if the namespace URI is invalid
    - ELEMENT_NOT_FOUND if the element at elementPath does not exist
    - INVALID_XPATH if elementPath is not a well-formed XPath-expression
    - ELEMENT_PATH_NOT_UNIQUE if elementPath resolves not to a single element but to a list of elements
    - FAILED If the namespace could not be set due to another error.
 */
DLL_EXPORT ReturnCode tixiSetElementNamespace(const TixiDocumentHandle handle, const char* elementPath,
                                              const char* namespaceURI, const char* prefix);


/**
  @brief This function adds a namespace declaration inside the tag of specified element.

  This function can be used, after an element was created or read
  from file to set the namespace of the element.

  The namespace is not applied to the element, but can be used from child nodes.

  Different to ::tixiSetElementNamespace the namespace prefix may not be empty!

  <b>Fortran syntax:</b>

  tixi_add_namespace_attribute( integer handle, character*n element_path, character*n namespace_uri, character*n prefix, integer error )

  @return
    - SUCCESS if the namespace could be set successfully
    - INVALID_HANDLE if the handle is not valid, i.e.  does not or no longer exist
    - INVALID_NAMESPACE_URI if the namespace URI is invalid
    - ELEMENT_NOT_FOUND if the element at elementPath does not exist
    - INVALID_XPATH if elementPath is not a well-formed XPath-expression
    - ELEMENT_PATH_NOT_UNIQUE if elementPath resolves not to a single element but to a list of elements
    - FAILED If the prefix is empty or the namespace could not be set due to another error.
 */
DLL_EXPORT ReturnCode tixiDeclareNamespace(const TixiDocumentHandle handle, const char* elementPath,
                                           const char* namespaceURI, const char* prefix);

/*@}*/
/**
  \defgroup MiscFunctions Miscellaneous Functions

  These function simply do not fit into one of the other categories.
 */
/*@{*/


/**
  @brief Add a name of an external file as a url.

  <b>Fortran syntax:</b>

  tixi_add_external_link( integer  handle, character*n parent_path, character*n url, character*n file_format, integer error )

  @param[in]  handle as returned by ::tixiCreateDocument
  @param[in]  parentPath path to the element into which the element holding the url
                         should be inserted.
  @param[in]  url an url to specify an additional output file not in XML-format
  @param[in]  fileFormat an optional attribute (may be NULL) to specify a file format,
                         e.g. CNGS, netcdf, ...

  @return
    - SUCCESS if successfully added the header
    - FAILED if an internal error occured
    - INVALID_HANDLE if the handle is not valid
    - ALREADY_SAVED if the header should be added to an already saved document
 */
DLL_EXPORT ReturnCode tixiAddExternalLink (const TixiDocumentHandle handle, const char *parentPath,
                                           const char *url, const char *fileFormat);


/**
  @brief Add header to XML-file.

  Inserts a header containing information on the tool used to create the file, its
  version and the user. Additionally, the TIXI version is inserted. If an strings
  equals to NULL an empty element is inserted. This routine shold be called right after
  ::tixiCreateDocument.

  <b>Fortran syntax:</b>

  tixi_add_header( integer handle, character*n tool_name, character*n version, character*n author_name, integer error )

  @param[in]  handle as returned by ::tixiCreateDocument
  @param[in]  toolName name of the tool used to write the file
  @param[in]  authorName string to identify the creator of the file
  @param[in]  version string to identify the version of the tool

  @return
    - SUCCESS if successfully added the header
    - FAILED if an internal error occured
    - INVALID_HANDLE if the handle is not valid
    - ALREADY_SAVED if the header should be added to an already saved document
 */
DLL_EXPORT ReturnCode tixiAddHeader (const TixiDocumentHandle handle, const char *toolName,
                                     const char *version, const char *authorName);



/**
  @brief Add CPACS header to XML-file.

  Inserts a header containing information on the data set in this file.
  If an strings equals to NULL an empty element is inserted. This routine should be called right after
  ::tixiCreateDocument. An timestamp is automaticly added to the header.

  <b>Fortran syntax:</b>

  tixi_add_cpacs_header( integer handle, character*n name, character*n creator, character*n version, character*n descripton, character*n cpacs_version, integer error )

  @param[in]  handle as returned by ::tixiCreateDocument
  @param[in]  name   name of the data set
  @param[in]  creator string to identify the creator of the file
  @param[in]  version string to identify the version of the file
  @param[in]  description  optional string to set a description to the file
  @param[in]  cpacsVersion CPACS version number

  @return
    - SUCCESS if successfully added the header
    - FAILED if an internal error occured
    - INVALID_HANDLE if the handle is not valid
    - ALREADY_SAVED if the header should be added to an already saved document
 */
DLL_EXPORT ReturnCode tixiAddCpacsHeader (const TixiDocumentHandle handle, const char *name, const char *creator,
                                          const char *version, const char *description, const char * cpacsVersion);


/**
  @brief Checks if the given element exists.

  @param[in]  handle handle as returned by ::tixiCreateDocument, ::tixiOpenDocumentRecursive or ::tixiOpenDocumentFromHTTP
  @param[in]  elementPath an XPath compliant path to an element in the document
                          specified by handle (see section \ref XPathExamples above).

  @return
    - SUCCESS if the element exists
    - INVALID_HANDLE if the handle is not valid, i.e.  does not or no longer exist
    - FAILED internal error
    - INVALID_XPATH  if elementPath is not a well-formed XPath-expression
    - ELEMENT_NOT_FOUND if the elementPath does not point to an existing element
 */
DLL_EXPORT ReturnCode tixiCheckElement (const TixiDocumentHandle handle, const char *elementPath);


/**
  @brief Checks for validity of a document handle

  <b>Fortran syntax:</b>

  tixi_check_handle( integer handle )

  @param[in]  handle  handle as returned by ::tixiCreateDocument, ::tixiOpenDocumentRecursive or ::tixiOpenDocumentFromHTTP

  @return
    - SUCCESS if handle is valid
    - INVALID_HANDLE if the handle is not valid, i.e. does not or no longer exist
 */
DLL_EXPORT ReturnCode tixiCheckDocumentHandle (const TixiDocumentHandle handle);

/**
  @brief Sets Pretty print on or off.

  Set pretty print on or off. This is used when saing a document to a file,
  or when exporting to a string.
  0 turns pretty print off, 1 turns pretty print on.
  By default, pretty print is turned on.h

  <b>Fortran syntax:</b>

  tixi_use_pretty_print( integer  handle, integer use_pretty_print, integer error )

  @param[in]  handle  handle as returned by ::tixiOpenDocument
  @param[in]  usePrettyPrint flag if output should be pretty printed

  @return
    - SUCCESS if a count is computed
    - INVALID_HANDLE if the handle is not valid, i.e.  does not or no longer exist
    - FAILED for internal errors or if usePrettyPrint had wrong value
 */
DLL_EXPORT ReturnCode tixiUsePrettyPrint(TixiDocumentHandle handle, int usePrettyPrint);

/**
  @brief Reroutes all messages of tixi to the message function func

  This can be used, to rerout all tixi messages to a log file, to modify
  the messages, to parse them etc...

  <b>Example to keep tixi silent:</b>
  @code{.c}
  // define the message sink
  void tixiSilentMessage(MessageType , const char *, ...){}

  // set the message sink
  tixiSetPrintMsgFunc(tixiSilentMessage);
  @endcode

  @param[in]  func The new message receiver function

  @return
    - SUCCESS if func is valid
    - FAILED  if func is a null pointer
 */
DLL_EXPORT ReturnCode tixiSetPrintMsgFunc(TixiPrintMsgFnc func);

/**
  @brief Returns the currently registered message handler function
  
  @return:
    - Function pointer to the message handler
 */
DLL_EXPORT TixiPrintMsgFnc tixiGetPrintMsgFunc();

/*@}*/

/**
  \defgroup HighLevelFunctions High Level Functions

  These functions operate on more complex data structures than the elementary get/add function so.
 */
/*@{*/

/**
  @brief High level routine to write a list of elements with attributes.

  Adds a list of elements with the same name containd in an element with the name listName to the element
  specified by parentPath.

  <b>Fortran syntax:</b>

  tixi_add_double_list_with_attributes( integer  handle, character*n parent_path, character*n list_name, character*n child_name, character*n child_attribute_name, real values, character*n format, character*n attributes, integer n_values, integer error )

  @param[in]  handle file handle as returned by ::tixiCreateDocument

  @param[in]  parentPath an XPath compliant path to an element in the
                         document specified by handle (see section \ref XPathExamples above).

  @param[in]  listName name of the element containing the list

  @param[in]  childName name of the child elements of the element listName

  @param[in]  childAttributeName name of the attribute to be assigned to each child

  @param[in]  values array of double holding the element values to be added ("text" content between the tags)

  @param[in]  format format string used to convert number into a string.
                     The format string usage is identical to format strings in printf.
                     If format is NULL "%g" will be used to format the string.

  @param[in]  attributes array of pointers to strings holding the attribute values (vs. attribute name which is the same for each list entry)

  @param[in]  nValues number of values in the list

  @return
    - SUCCESS if the list has been added successfully
    - FAILED if an internal error occured
    - INVALID_HANDLE if the handle is not valid, i.e.  does not or no longer exist
    - INVALID_XPATH if parentPath is not a well-formed XPath-expression
    - ELEMENT_NOT_FOUND if parentPath does not point to a node in the XML-document
    - ELEMENT_PATH_NOT_UNIQUE if parentPath resolves not to a single element
 */
DLL_EXPORT ReturnCode tixiAddDoubleListWithAttributes (const TixiDocumentHandle handle,
                                                       const char *parentPath,
                                                       const char *listName, const char *childName,
                                                       const char *childAttributeName,
                                                       const double *values, const char *format,
                                                       const char **attributes, int nValues);

/**
  @brief Retrieves the size of an Vector.

  Returns the size of semicolon separated elementf in an vector. The node containung
  the vector has to be tagged via the xml attribute <<mapType="vector">>.

  tixi_get_vector_size( integer  handle,  character*n vectorPath, integer nElements, integer error)

  @param[in]  handle file handle as returned by ::tixiOpenDocument, ::tixiOpenDocumentRecursive, ::tixiOpenDocumentFromHTTP, ::tixiCreateDocument or ::tixiImportFromString
  @param[in]  vectorPath an XPath compliant path to an element holding the vector in
                         the document specified by handle (see section \ref XPathExamples).
  @param[out] nElements  number of vector elements

  @return
    - SUCCESS if successfully read the matrix element
    - FAILED internal error
    - INVALID_HANDLE if the handle is not valid
    - INVALID_XPATH if matrixPath is not a well-formed XPath-expression
    - ELEMENT_PATH_NOT_UNIQUE if matrixPath resolves not to a single element but to a list of elements
    - ELEMENT_NOT_FOUND if arrayPath points to a element that is no array
 */
DLL_EXPORT ReturnCode tixiGetVectorSize (const TixiDocumentHandle handle,
                                         const char *vectorPath, int *nElements);


/**
  @brief Retrieves a vector.

  A vector is read and its contents are stored into an 1D-array. The memory necessary
  for the array is automatically allocated. The number of elements in the vector
  could be read via a call to "tixiGetVectorSize".

  tixi_get_float_vector( integer handle, character*n vectorPath, real array, integer eNumber)

  @param[in]  handle file handle as returned by ::tixiOpenDocument, ::tixiOpenDocumentRecursive, ::tixiOpenDocumentFromHTTP, ::tixiCreateDocument or ::tixiImportFromString
  @param[in]  vectorPath an XPath compliant path to an element holding the vector in
                         the document specified by handle (see section \ref XPathExamples).
  @param[out] vectorArray a pointer address for an array that should hold the vector elements
  @param[in]  eNumber maximal number of elements that shout be read from this vector.

  @return
    - SUCCESS if successfully read the vector
    - FAILED internal error
    - INVALID_HANDLE if the handle is not valid
    - INVALID_XPATH if matrixPath is not a well-formed XPath-expression
    - ELEMENT_PATH_NOT_UNIQUE if vectorPath resolves not to a single element but to a list of elements
    - ELEMENT_NOT_FOUND if vectorPath points to a non-existing element

  @cond
  #annotate out: 2A(3) # the size of the output array vectorArray is determined by "eNumber"
  @endcond
    */
DLL_EXPORT ReturnCode tixiGetFloatVector (const TixiDocumentHandle handle, const char *vectorPath,
                                          double **vectorArray, const int eNumber);


/**
  @brief Retrieves the number of dimensions of an Array.

  Returns the number of separate dimensions defined in sub-tags of the array in CPACS.
  For each dimension there is a finite set of allowed values that can be retrieved by
  "tixiGetArrayValues".

  Example:

  @code{.xml}
  <aeroPerformanceMap>
    <machNumber mapType="vector">0.2;0.6</machNumber>
    <reynoldsNumber mapType="vector">10000000;30000000</reynoldsNumber>
    <angleOfYaw mapType="vector">-5.;0;15</angleOfYaw>
    <angleOfAttack mapType="vector">-4.;-2.;0.;2.;4.;6.;8.;10.;12.;14.;16.</angleOfAttack>
    <cfx mapType="array">...</array>
    <cfy mapType="array">...</array>
    <cfz mapType="array"/>
    <cmx mapType="array"/>
    <cmy mapType="array"/>
    <cmz mapType="array"/>
  </aeroPerformanceMap>
  @endcode

  This is a typical array definition, to be found under, e.g., the xpath "//aeroPerformanceMap":
  - All direct child tags with the attribute 'mapType="vector"' define one dimension each, containing several ";" separated values.
  - All direct child tags with the attribute 'mapType="array"' define a list of values, the list having a size of the
    cross product of all dimensions' sizes (in this example 2*2*3*11 = 132 elements per "array" list).

  <b>Fortran syntax:</b>

  tixi_get_array_dimensions( integer handle, character*n arrayPath, integer* dimensions )

  @param[in]  handle as returned by ::tixiCreateDocument
  @param[in]  arrayPath an XPath-compliant path to an element holding the sub elements that define the dimensions in
                        the document specified by the handle (see section \ref XPathExamples).
  @param[out] dimensions number of array dimensions

  @return
    - SUCCESS if the dimensions have been successfully read
    - FAILED for internal errors
    - INVALID_HANDLE if the handle is not valid
    - INVALID_XPATH if arrayPath is not a well-formed XPath-expression
    - ELEMENT_PATH_NOT_UNIQUE if arrayPath resolves not to a single element but to a list of elements
    - ELEMENT_NOT_FOUND if the rrayPath points to a element that is no array
 */
DLL_EXPORT ReturnCode tixiGetArrayDimensions (const TixiDocumentHandle handle,
                                              const char *arrayPath, int *dimensions);


/**
  @brief Retrieves the sizes of all dimensions of the array definition, and the overall array size (product of all dimensions's sizes).

  For an array use example, please check tixiGetArrayDimensions()

  <b>Fortran syntax:</b>

  tixi_get_array_dimension_sizes ( integer handle, character*n arrayPath, int*n sizes, int* arraySizes )

  @param[in]  handle as returned by ::tixiCreateDocument
  @param[in]  arrayPath an XPath-compliant path to the top XML element holding the sub-tags of the array definition in
                        the document specified by the handle (see section \ref XPathExamples).
  @param[out] sizes an integer array containing the size of each dimension
  @param[out] linearArraySize product over all sizes (for complete array size)

  @return
    - SUCCESS if the dimensions have been successfully read
    - FAILED internal error
    - INVALID_HANDLE if the handle is not valid
    - INVALID_XPATH if matrixPath is not a well-formed XPath-expression
    - ELEMENT_PATH_NOT_UNIQUE if matrixPath resolves not to a single element but to a list of elements
    - ELEMENT_NOT_FOUND if the arrayPath points to a element that is no array

  @cond
  #annotate out: 2AM# sizes is an output array that has to be manually preallocated
  @endcond
 */
DLL_EXPORT ReturnCode tixiGetArrayDimensionSizes (const TixiDocumentHandle handle, const char *arrayPath,
                                                  int *sizes, int *linearArraySize);


/**
  @brief Retrieves the names of all dimensions.

  For an array use example, please check tixiGetArrayDimensions()

  <b>Fortran syntax:</b>

  tixi_get_array_dimension_names ( integer handle, character*n arrayPath, char*n*m dimensionNames)

  @param[in]  handle file handle as returned by ::tixiOpenDocument, ::tixiOpenDocumentRecursive, ::tixiOpenDocumentFromHTTP, ::tixiCreateDocument or ::tixiImportFromString
  @param[in]  arrayPath an XPath compliant path to an element holding the sub-tags of the array definition in
                        the document specified by handle (see section \ref XPathExamples).
  @param[out] dimensionNames array of strings

  @return
    - SUCCESS if successfully read the matrix element
    - FAILED internal error
    - INVALID_HANDLE if the handle is not valid
    - INVALID_XPATH if matrixPath is not a well-formed XPath-expression
    - ELEMENT_PATH_NOT_UNIQUE if matrixPath resolves not to a single element but to a list of elements
    - ELEMENT_NOT_FOUND if arrayPath points to a element that is no array

  @cond
  #annotate out: 2AM# one user specified return array (of strings)
  @endcond
 */
DLL_EXPORT ReturnCode tixiGetArrayDimensionNames (const TixiDocumentHandle handle,
                                                  const char *arrayPath, char **dimensionNames);

/**
  @brief Retrieves the selected dimension's values (e.g. separate allowed angles etc.).

  For an array use example, please check tixiGetArrayDimensions()

  <b>Fortran syntax:</b>

  tixi_get_array_dimension_values ( integer handle, character*n arrayPath, int* dimension, double *dimensionValues)

  @param[in]  handle file handle as returned by ::tixiOpenDocument, ::tixiOpenDocumentRecursive, ::tixiOpenDocumentFromHTTP, ::tixiCreateDocument or ::tixiImportFromString
  @param[in]  arrayPath an XPath compliant path to an element holding the sub-tags of the array definition in
                        the document specified by handle (see section \ref XPathExamples).
  @param[in]  dimension which dimension to return meaning values for
  @param[out] dimensionValues all values for this dimension. The return array's Size should be determined by a prior call to tixiGetArrayDimensionSizes()

  @return
    - SUCCESS if successfully read the matrix element
    - FAILED internal error
    - INVALID_HANDLE if the handle is not valid
    - INVALID_XPATH if matrixPath is not a well-formed XPath-expression
    - ELEMENT_PATH_NOT_UNIQUE if matrixPath resolves not to a single element but to a list of elements
    - ELEMENT_NOT_FOUND if arrayPath points to a element that is no array
  @cond
  #annotate out: 3AM# the size of the array "dimensionValues" is determined by a prior call to tixiGetArrayDimensionSizes
  @endcond
 */
DLL_EXPORT ReturnCode tixiGetArrayDimensionValues (const TixiDocumentHandle handle, const char *arrayPath,
                                                   const int dimension, double *dimensionValues);


/**
  @brief Retrieves the number of parameters of an array.

  For an array use example, please check tixiGetArrayDimensions()

  Returns the number of different parameters (cutting all dimensions) defined in sub-tags in CPACS.
  Parameters are the tags with the attribute 'mapType="array"'.

  <b>Fortran syntax:</b>

  tixi_get_array_parameters( integer handle, character*n arrayPath, integer* parameters )

  @param[in]  handle as returned by ::tixiCreateDocument
  @param[in]  arrayPath an XPath compliant path to an element holding the sub elements that define the dimensions in
                        the document specified by handle (see section \ref XPathExamples).
  @param[out] parameters number of array parameters

  @return
    - SUCCESS if successfully read the dimensions
    - FAILED internal error
    - INVALID_HANDLE if the handle is not valid
    - INVALID_XPATH if arrayPath is not a well-formed XPath-expression
    - ELEMENT_PATH_NOT_UNIQUE if arrayPath resolves not to a single element but to a list of elements
    - ELEMENT_NOT_FOUND if arrayPath points to a element that is no array
 */
DLL_EXPORT ReturnCode tixiGetArrayParameters (const TixiDocumentHandle handle, const char *arrayPath, int *parameters);


/**
  @brief Retrieves names of all parameters

  For an array use example, please check tixiGetArrayDimensions()

  <b>Fortran syntax:</b>

  tixi_get_array_parameter_names ( integer handle, character*n arrayPath, character*n*m parameterNames)

  @param[in]  handle as returned by ::tixiCreateDocument
  @param[in]  arrayPath an XPath compliant path to an element holding the sub-tags of the array definition in
                        the document specified by handle (see section \ref XPathExamples).
  @param[out] parameterNames string array containing names of each parameter

  @return
    - SUCCESS if successfully read the matrix element
    - FAILED internal error
    - INVALID_HANDLE if the handle is not valid
    - INVALID_XPATH if matrixPath is not a well-formed XPath-expression
    - ELEMENT_PATH_NOT_UNIQUE if matrixPath resolves not to a single element but to a list of elements
    - ELEMENT_NOT_FOUND if arrayPath points to a element that is no array
  @cond
  #annotate out: 2AM# one user specified return array (of strings)
  @endcond
 */
DLL_EXPORT ReturnCode tixiGetArrayParameterNames (const TixiDocumentHandle handle,
                                                  const char *arrayPath, char **parameterNames);


/**
  @brief Reads in an array. The memory management of the array is done by tixi.

  For an array use example, please check ::tixiGetArrayDimensions()

  <b>Fortran syntax:</b>

  tixi_get_array ( integer handle, character*n arrayPath, character*n element, double *values)

  @param[in]  handle file handle as returned by ::tixiOpenDocument, ::tixiOpenDocumentRecursive, ::tixiOpenDocumentFromHTTP, ::tixiCreateDocument or ::tixiImportFromString
  @param[in]  arrayPath an XPath compliant path to an element holding the sub-tags of the array definition in
                        the document specified by handle (see section \ref XPathExamples).
  @param[in]  elementName name of the sub tag that contains the array
  @param[in]  arraySize Total size of the array. Size must be determined by calling ::tixiGetArrayDimensionSizes and must equal
                        the product of all dimensions' sizes
  @param[out] values Pointer to a double array, containg all values for this dimension. The array is allocated and freed
                      by tixi. The size of the array corresponds to the parameter arraySize

  @return
    - SUCCESS if successfully read the matrix element
    - FAILED internal error
    - INVALID_HANDLE if the handle is not valid
    - INVALID_XPATH if matrixPath is not a well-formed XPath-expression
    - ATTRIBUTE_NOT_FOUND if the given sub element has not mapType="array"
    - ELEMENT_PATH_NOT_UNIQUE if matrixPath resolves not to a single element but to a list of elements
    - ELEMENT_NOT_FOUND if arrayPath points to a element that is no array
    - NON_MATCHING_SIZE if arraySize does not match number of read elements in specified array

  @cond
  #annotate out: 4A(3) # the size of the array "values" is determined by arraySize
  @endcond
 */
DLL_EXPORT ReturnCode tixiGetArray (const TixiDocumentHandle handle, const char *arrayPath,
                                    const char *elementName, int arraySize, double **values);


/**
  @brief Getter function to take one multidimensionally specified element from a complete array, retrieved earlier.

  For an array use example, please check tixiGetArrayDimensions()

  <b>Fortran syntax:</b>

  tixi_get_array_value ( double*n array, integer*n dimSize, integer*n dimPos, integer *dims)

  @param[in]  array the array as returned by ::tixiGetArray()
  @param[in]  dimSize the array of dimensions' sizes as returned by ::tixiGetArrayDimensionSizes()
  @param[in]  dimPos the index of each dimension to fetch from the array
  @param[in]  dims the number of dimensions of the array as returned by ::tixiGetArrayDimensions()

  @return The element fetched
*/
DLL_EXPORT double tixiGetArrayValue(const double *array, const int *dimSize, const int *dimPos, const int dims);


/**
  @brief Helper function.

  Returns the number of sub elements in CPACS arrays (either vector or array type).

  For an array use example, please check ::tixiGetArrayDimensions(). The result for elementName="angleOfAttack" would be 11.

  <b>Fortran syntax:</b>

  tixi_get_array_element_count( integer handle, character*n arrayPath, character*n elementName, integer* elements )

  @param[in]  handle as returned by ::tixiCreateDocument
  @param[in]  arrayPath an XPath compliant path to an element holding the sub elements that define the dimensions in
                        the document specified by handle (see section \ref XPathExamples).
  @param[in]  elementName The name of the sub element under the given xpath.
  @param[out] elements number of array parameters (separated by ";")

  @return
    - SUCCESS if successfully read the dimensions
    - FAILED internal error
    - INVALID_HANDLE if the handle is not valid
    - INVALID_XPATH if arrayPath is not a well-formed XPath-expression
    - ELEMENT_PATH_NOT_UNIQUE if arrayPath resolves not to a single element but to a list of elements
    - ELEMENT_NOT_FOUND if arrayPath points to a element that is no array
 */
DLL_EXPORT ReturnCode tixiGetArrayElementCount (const TixiDocumentHandle handle, const char *arrayPath,
                                                const char *elementName, int *elements);


/**
  @brief Helper function.

  Returns the tag names of sub elements of mapType given.

  For an array use example, please check tixiGetArrayDimensions()

  <b>Fortran syntax:</b>

  tixi_get_array_element_names( integer handle, character*n arrayPath, character*n elementName, character*n*m elementNames)

  @param[in]  handle as returned by ::tixiCreateDocument
  @param[in]  arrayPath an XPath compliant path to an element holding the sub elements that define the dimensions in
                        the document specified by handle (see section \ref XPathExamples).
  @param[in]  elementType mapType to get names for (either "vector" for a dimension or "array" for the data field)
  @param[out] elementNames string names of all tags found for the given type.

  @return
    - SUCCESS if successfully read the dimensions
    - FAILED internal error
    - INVALID_HANDLE if the handle is not valid
    - INVALID_XPATH if arrayPath is not a well-formed XPath-expression
    - ELEMENT_PATH_NOT_UNIQUE if arrayPath resolves not to a single element but to a list of elements
    - ELEMENT_NOT_FOUND if arrayPath points to a element that is no array
 */
DLL_EXPORT ReturnCode tixiGetArrayElementNames (const TixiDocumentHandle handle, const char *arrayPath,
                                                const char *elementType, char **elementNames);


/**
  @brief Adds an element containing the 3D cartesian coordinates of a point.

  An element with the following structure is added:

  @verbatim
    <x> </x>
    <y> </y>
    <z> </z>
  @endverbatim

  <b>Fortran syntax:</b>

  tixi_add_point( integer  handle,  character*n pointParentPath,
                  real x, real y, real z, character*n format, integer error )

  @param[in]  handle as returned by ::tixiOpenDocument, ::tixiOpenDocumentRecursive, ::tixiOpenDocumentFromHTTP, ::tixiCreateDocument or ::tixiImportFromString
  @param[in]  pointParentPath an XPath compliant path to an element into
                             which the point elements are to be inserted in the document
                             specified by handle (see section \ref XPathExamples).
  @param[in]  x x coordinate of the point
  @param[in]  y y coordinate of the point
  @param[in]  z z coordinate of the point
  @param[in]  format format string used to convert number into a string.
                     The format string usage is identical to format strings in printf.
                     If format is NULL "%g" will be used to format the string.

  @return
    - SUCCESS if successfully added the point element
    - INVALID_HANDLE if the handle is not valid
    - INVALID_XPATH if pointParentPath is not a well-formed XPath-expression
    - ELEMENT_PATH_NOT_UNIQUE if pointParentPath resolves not to a single element but to a list of elements
    - ELEMENT_NOT_FOUND if pointParentPath points to a non-existing element
    - ALREADY_SAVED if element should be added to an already saved document
 */
DLL_EXPORT ReturnCode tixiAddPoint (const TixiDocumentHandle handle, const char *pointParentPath,
                                    double x, double y, double z, const char *format);

/**
  @brief Reads a point element but ignores error if an incomplete point is encountered.

  An element with the following structure is expected as child of the
  element specified by pointParentPath

  @verbatim
    <x> </x>
    <y> </y>
    <z> </z>
  @endverbatim

  @brief Reads a point element.

  An element with the following structure is expected as child of the element specified by pointParentPath.
  Some or even all coordinate entries may be missing. The value of a
  missing coordiante is unchanged on output. This feature can be used to provide default values
  for non existing coordinate elements.

  The coordinates are returned in the x, y, and z arguments.

  <b>Fortran syntax:</b>

  tixi_get_point(  integer  handle,  character*n pointParentPath, integer index,
                   real x, real y, real z, integer error )

  @param[in]  handle as returned by ::tixiOpenDocument
  @param[in]  pointParentPath an XPath compliant path to an element containing point elements
                              in the document specified by handle (see section \ref XPathExamples).
  @param[out] x x coordinate of the point
  @param[out] y y coordinate of the point
  @param[out] z z coordinate of the point

  @return
     - SUCCESS if the content of the point element is retrieved successfully
     - INVALID_HANDLE if the handle is not valid, i.e.  does not or no longer exist
     - INVALID_XPATH if pointParentPath is not a well-formed XPath-expression
     - ELEMENT_NOT_FOUND if pointParentPath does not point to a node in the XML-document
     - ELEMENT_PATH_NOT_UNIQUE if pointParentPath resolves not to a single element but to a list of elements
 */
DLL_EXPORT ReturnCode tixiGetPoint (const TixiDocumentHandle handle,
                                    const char *pointParentPath,
                                    double *x, double *y, double *z);



/**
  @brief Checks the existence of an element's attribute.

  <b>Fortran syntax:</b>

  tixi_check_attribute( integer  handle, character*n element_path, character*n attribute_name, integer error )

  @param[in]  handle handle as returned by ::tixiOpenDocument, ::tixiOpenDocumentRecursive, ::tixiOpenDocumentFromHTTP, ::tixiCreateDocument or ::tixiImportFromString

  @param[in]  elementPath an XPath compliant path to an element in the document
                          specified by handle (see section \ref XPathExamples above).

  @param[in]  attributeName name of the attribute to be added to the element

  @return
    - SUCCESS if successfully retrieve the text content of a single element
    - INVALID_HANDLE if the handle is not valid, i.e.  does not or no longer exist
    - INVALID_XPATH if elementPath is not a well-formed XPath-expression
    - ATTRIBUTE_NOT_FOUND if the element has no attribute attributeName
    - ELEMENT_NOT_FOUND if elementPath does not point to a node in the XML-document
    - ELEMENT_PATH_NOT_UNIQUE if elementPath resolves not to a single element but to a list of elements
 */
DLL_EXPORT ReturnCode tixiCheckAttribute(TixiDocumentHandle handle, const char *elementPath, const char *attributeName);


/*@}*/
/**
    \defgroup XSL XSLT Functions
      Function to perform XSL transformations.
   */
/*@{*/

/**
  @brief Performs a XML transformation .

  <b>Fortran syntax:</b>

  tixi_xsl_transformation_to_string( integer  handle, character*n xslFilename, character*n resultFilename, integer error )


  @param[in]  handle handle as returned by ::tixiOpenDocument, ::tixiOpenDocumentRecursive, ::tixiOpenDocumentFromHTTP, ::tixiCreateDocument or ::tixiImportFromString
  @param[in]  xslFilename The name of a local file with XSL mapping informations.
  @param[out] resultText The resulting document of the transformation.
  @return
    - SUCCESS if successful
    - FAILED  is something went wrong, i.e. opening of files
 */
DLL_EXPORT ReturnCode tixiXSLTransformationToString(TixiDocumentHandle handle, const char *xslFilename, char **resultText);

/**
  @brief Performs a XML transformation and saves the result to resultFilename.

  <b>Fortran syntax:</b>

  tixi_xsl_transformation_to_file( integer  handle, character*n xslFilename, character*n resultFilename, integer error )


  @param[in]  handle handle as returned by ::tixiOpenDocument, ::tixiOpenDocumentRecursive, ::tixiOpenDocumentFromHTTP, ::tixiCreateDocument or ::tixiImportFromString
  @param[in]  xslFilename The name of a local file with XSL mapping informations.
  @param[in]  resultFilename The name of a local file to store the resulting XML document in.
  @return
    - SUCCESS if successful
    - FAILED  is something went wrong, i.e. opening of files
 */
DLL_EXPORT ReturnCode tixiXSLTransformationToFile(TixiDocumentHandle handle, const char *xslFilename, const char *resultFilename);


/*@}*/
/**
  \defgroup XPATH XPath Utility Functions

  Function for evaluating XPath expressions.
     */
/*@{*/

/**
  @brief Evaluates a XPath expression and returns the number of result nodes matching this xpath expression.

  <b>Fortran syntax:</b>

  tixi_xpath_evaluate_node_number( integer handle, character*n xpathExpression, integer number, integer error )

  @param[in]  handle handle as returned by ::tixiOpenDocument, ::tixiOpenDocumentRecursive, ::tixiOpenDocumentFromHTTP, ::tixiCreateDocument or ::tixiImportFromString
  @param[in]  xPathExpression The XPath Expression to evaluate.
  @param[out] number The number of nodes matching this xpath expression.

  @return
    - SUCCESS if successfully retrieved the number of the xPath search.
    - FAILED  if an internal error occured.
 */
DLL_EXPORT ReturnCode tixiXPathEvaluateNodeNumber(TixiDocumentHandle handle, const char *xPathExpression, int *number);

/**
  @brief Evaluates a XPath expression and the xPath for the i-th result.

  @param[in]  handle handle as returned by ::tixiOpenDocument, ::tixiOpenDocumentRecursive, ::tixiOpenDocumentFromHTTP, ::tixiCreateDocument or ::tixiImportFromString
  @param[in]  xPathExpression The XPath Expression to evaluate.
  @param[in]  index The index of the result to query, with 1 <= index <= number, and number queried with ::tixiXPathEvaluateNodeNumber
  @param[out] xPath The xPath of the search result.

  @return
    - SUCCESS if successfully retrieve the xPath of the search.
    - INVALID_HANDLE if the handle is not valid, i.e.  does not or no longer exist.
    - FAILED  if an internal error occured.
 */
DLL_EXPORT ReturnCode tixiXPathExpressionGetXPath(TixiDocumentHandle handle, const char *xPathExpression, int index, char** xPath);

/**
  @brief Evaluates a XPath expression and returns the text content of the resultnode matching this xpath expression.

  The element to get is selected by an index number, which should be between 1 and 'tixiXPathEvaluateNodeNumber() (inclusively)'.
  On successful return the memory used for text is allocated
  internally and must not be released by the user. The deallocation
  is handle when the document referred to by handle is closed.

  <b>Fortran syntax:</b>

  tixi_xpath_expression_get_text_by_index( integer handle, character*n xpathERxpression, integer index, character*n text, integer error )

  @param[in]  handle handle as returned by ::tixiOpenDocument, ::tixiOpenDocumentRecursive, ::tixiOpenDocumentFromHTTP, ::tixiCreateDocument or ::tixiImportFromString
  @param[in]  xPathExpression The XPath Expression to evaluate.
  @param[in]  elementNumber The (index)-number to get. Needed if the XPath expression catches more than one node. Index must
                            be between 1 and 'tixiXPathEvaluateNodeNumber()', inclusively.
  @param[out] text the text content of the node-value. Works for xml-elements and attributes. NULL if an error occured.

  @return
    - SUCCESS if successfully retrieve the text content of a single element.
    - INVALID_XPATH If something goes wrong on evaluating the xpath expression.
    - FAILED  is an internal error occured.
 */
DLL_EXPORT ReturnCode tixiXPathExpressionGetTextByIndex(TixiDocumentHandle handle, const char *xPathExpression, int elementNumber, char **text);


/*@}*/
/**
  \defgroup UID UID Helper Functions

  Function for UID management.
 */
/*@{*/

/**
  @brief Performs a check over all UIDs and checks for duplicates.

  <b>Fortran syntax:</b>

  tixi_uid_check_duplicates( integer  handle, integer error )

  @param[in]  handle handle as returned by ::tixiOpenDocument, ::tixiOpenDocumentRecursive, ::tixiOpenDocumentFromHTTP, ::tixiCreateDocument or ::tixiImportFromString

  @return
    - SUCCESS if all UIDs are unique
    - FAILED if reading of uids does not work
    - UID_NOT_UNIQUE  is there is a not unique UID
 */
DLL_EXPORT ReturnCode tixiUIDCheckDuplicates(TixiDocumentHandle handle);


/**
  @brief Performs a check over all nodes with the uID "isLink" and check if the corresponding uid exists in that data set.

  <b>Fortran syntax:</b>

  tixi_uid_check_links( integer  handle, integer error )

  @param[in]  handle handle as returned by ::tixiOpenDocument, ::tixiOpenDocumentRecursive, ::tixiOpenDocumentFromHTTP, ::tixiCreateDocument or ::tixiImportFromString

  @return
    - SUCCESS if all links are valid
    - FAILED if reading of links or uids does not work
    - UID_NOT_UNIQUE  is there is a not unique UID
    - UID_LINK_BROKEN if there is a broken link.
 */
DLL_EXPORT ReturnCode tixiUIDCheckLinks(TixiDocumentHandle handle);

/**
  @brief Returns the XPath to given uID.

  On successful return the memory used for text is allocated
  internally and must not be released by the user. The deallocation
  is handle when the document referred to by handle is closed.

  <b>Fortran syntax:</b>

  tixi_uid_get_xpath( integer  handle, character*n uid, character*n text, integer error )

  @param[in]  handle    handle as returned by ::tixiOpenDocument, ::tixiOpenDocumentRecursive, ::tixiOpenDocumentFromHTTP, ::tixiCreateDocument or ::tixiImportFromString
  @param[in]  uID       The uid of which the xpath should be returned
  @param[out] xPath     The XPath of the element with the right uid. Empty string if uid does not exist.

  @return
    - SUCCESS if all UIDs are unique
    - UID_NOT_UNIQUE  is there is a not unique UID
    - FAILED  for internal error
 */
DLL_EXPORT ReturnCode tixiUIDGetXPath(TixiDocumentHandle handle, const char *uID, char **xPath);


/**
  @brief Checks if a uID exists.

  <b>Fortran syntax:</b>

  tixi_uid_check_exists( integer  handle, character*n uid, integer error )

  @param[in]  handle    handle as returned by ::tixiOpenDocument, ::tixiOpenDocumentRecursive, ::tixiOpenDocumentFromHTTP, ::tixiCreateDocument or ::tixiImportFromString
  @param[in]  uID       The uid which should be checked

  @return
    - SUCCESS if all UIDs are unique
    - UID_DONT_EXISTS if the UID do not exist in the document
    - UID_NOT_UNIQUE  is there is a not unique UID
    - FAILED  for internal error
 */
DLL_EXPORT ReturnCode tixiUIDCheckExists(TixiDocumentHandle handle, const char *uID);


/**
  @brief Sets a uID attribute to a node, specified via its XPath.

  <b>Fortran syntax:</b>

  tixi_uid_set_to_xpath( integer  handle, character*n xpath, character*n uid, integer error )

  @param[in]  handle    handle as returned by ::tixiOpenDocument, ::tixiOpenDocumentRecursive, ::tixiOpenDocumentFromHTTP, ::tixiCreateDocument or ::tixiImportFromString
  @param[in]  xPath     The XPath to the node which should become the uid-attribute
  @param[in]  uID       The uid which should be checked

  @return
    - SUCCESS if all UIDs are unique
    - UID_DONT_EXISTS if the UID do not exist in the document
    - UID_NOT_UNIQUE  is there is a not unique UID
    - INVALID_HANDLE if the handle is not valid, i.e.  does not or no longer exist
    - INVALID_XPATH if elementPath is not a well-formed XPath-expression
    - ELEMENT_NOT_FOUND if elementPath does not point to a node in the XML-document
    - NO_ATTRIBUTE_NAME if attributeName is NULL
    - ELEMENT_PATH_NOT_UNIQUE if elementPath resolves not to a single element but to a list of elements
    - FAILED  for internal error
 */
DLL_EXPORT ReturnCode tixiUIDSetToXPath(TixiDocumentHandle handle, const char *xPath, const char *uID);


/*@}*/


#endif  /* TIXI_H */


#ifdef __cplusplus
}
#endif
