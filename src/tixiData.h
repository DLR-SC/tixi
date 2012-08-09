/* 
* Copyright (C) 2007-2011 German Aerospace Center (DLR/SC)
*
* Created: 2010-08-13 Hans-Peter Kersken <Hans-Peter.Kersken@dlr.de>
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
 * @brief  Declaration of the internal data structures used by tixi.
 */

#ifndef TIXIDATA_H
#define TIXIDATA_H

#include <string.h>
#include <stdio.h>

#include "libxml/xmlreader.h"

#define TIXI_VERSION  "2.0"

#define CPACS_UID_ATTRIBUTE_STRING 		"uID"
#define CPACS_UID_XPATH 				"//@uID"
#define CPACS_UID_LINK_XPATH			"//@isLink"
#define CPACS_UID_LINK_ATTRIBUTE_STRING	"isLink"

#define EXTERNAL_DATA_NODE_NAME				"externaldata"
#define EXTERNAL_DATA_NODE_NAME_PATH		"path"
#define EXTERNAL_DATA_NODE_NAME_FILENAME	"filename"

#define EXTERNAL_DATA_XML_ATTR_NODEPATH		 "externalDataNodePath"
#define EXTERNAL_DATA_XML_ATTR_FILENAME		 "externalFileName"
#define EXTERNAL_DATA_XML_ATTR_DIRECTORY	 "externalDataDirectory"


/**
 * Definitions about vectors and arrays
 */
#define VECTOR_SEPARATOR ";"
#define VECTOR_ATTRIBUTE_NAME "vector"
#define MAPTYPE_IDENTIFIER "mapType"




/**
 * Define debug levels
 */
#ifdef DEBUG
#define PRINT_DEBUG(format, arg) fprintf(stderr, format, arg);
#define PRINT_DEBUG2(format, arg1, arg2) fprintf(stderr, format, arg1, arg2);
#else
#define PRINT_DEBUG(format, arg1, arg2)
#define PRINT_DEBUG2(format, arg1, arg2)
#endif

/**
 * Add function for simple tracing
 */
#if WIN32
#define TRACE() PRINT_DEBUG("\n- TRACE : %s\n",  __FUNCTION__ )
#else
#define TRACE() PRINT_DEBUG("\n- TRACE : %s\n",  __func__ )
#endif

/**
  Possible values of tixiDocument.isValid.
 */
typedef enum ValidationStatusType
{
   INVALID,   /*!< Document not DTD or schema compliant */
   UNDEFINED, /*!< Document has not been checked        */
   VALID      /*!< Document is DTD or schema compliant */
} ValidationStatus;

/**
	Truth values.
*/

/**
  Possible values of tixiDocument.status.
 */
typedef  enum DocumentStatusType
{
   OPENED,   /**< The document was created by tixiCreateDocument or
                tixiOpenDocument and has been neither saved nor closed
                so far.*/
   SAVED,   /**< The document has been saved by tixiSaveDocument      */
   CLOSED   /**< The document has been closed by tixiCloseDocument     */
} DocumentStatus;


/**
 * @brief Structure to build a link list of documents.
 *
 */
typedef struct
{
      void* memory;                        /**< Pointer to memory area allocated by libxml */
      struct TixiMemoryListEntry* next;    /**< Pointer to next entry in the list */
} TixiMemoryListEntry;


/**
 * @brief Structure to build a link list of UIDs.
 *
 */
typedef struct
{
	xmlNodePtr nodePtr;						/**< Pointer to the node with the UID */
	char* uIDName;							/**< Name of this uID */
    struct TixiUIDListEntry* next;    		/**< Pointer to next entry in the list */
} TixiUIDListEntry;

/**
 * @brief Structure to hold the document meta data.
 *
 */
typedef struct
{
      char* xmlFilename;            /**< Full Name of the file associated with the document */
	  char* dirname;				/**< The name of the directory in which this master-cpacs-file is */
	  char* filename;				/**< The filename with(!) file extension */
      xmlDocPtr docPtr;             /**< libxml-pointer to document */
      xmlNodePtr currentNode;       /**< libxml-pointer to the current node */
      ValidationStatus isValid;     /**< Holds informations if and how the document has been validated */
      char* validationFilename;     /**< Name of the file the document is validate against */
      DocumentStatus status;        /**< Status of the document: opened/saved/closed */
      TixiDocumentHandle handle;
	  TixiMemoryListEntry* memoryListHead; /**< Pointer to the head of the list of memory location */
      TixiMemoryListEntry* memoryListTail; /**< Pointer to the tail of the list of memory location */
      int hasIncludedExternalFiles;        /**< Flag to indicate if the master file includes external files */
      int usePrettyPrint;                  /**< if pretty print should used when saving a file */
      TixiUIDListEntry* uidListHead;			   /**< Pointer to the head of the list of uids */
} TixiDocument;

typedef struct
{
      TixiDocument* document; /**< Document meta data */
      struct TixiDocumentListEntry* next;    /**< Pointer to next document in the list */
} TixiDocumentListEntry;

/**
   Pointer to the head of the list of documents managed by TIXI
 */
extern TixiDocumentListEntry* documentListHead;

/**
  Possible values how to save with/without external data.
 */
typedef enum InternalSaveModeType
{
   COMPLETE,   /**< The complete Document, with all external nodes is saved in one xml file. */
   SPLITTED,   /**< The document will be splitted in separated files. */
   REMOVED   /**< All external nodes are removed from the main document and only the reduced main file is saved. */
} InternalSaveMode;



#if WIN32
/* this is the name for mingw gcc's and MSVC */
#define VSNPRINTF _vsnprintf
#include <direct.h>
#define mkdir _mkdir
#else
/* this is the posix name */
#define VSNPRINTF vsnprintf
#include <libgen.h>
#endif

#endif /* TIXIDATA_H */
