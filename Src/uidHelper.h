/* 
* Copyright (C) 2007-2011 German Aerospace Center (DLR/SC)
*
* Created: 2010-08-13 Markus Litz <Markus.Litz@dlr.de>
* Changed: $Id: uidHelper.h 4392 2011-02-03 08:52:55Z litz_ma $ 
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
 * @file   uidHelper.h
 * @author Markus Litz <Markus.Litz@dlr.de>
 * @date   Tue Jul 25 12:06:33 2010
 *
 * @brief  Header file for helper routines with UIDs.
 *
 */
#include "libxml/parser.h"
#include "libxml/xpath.h"
#include "libxml/xmlsave.h"
#include "tixi.h"
#include "tixiData.h"

/**
 * Builds an internal structure of all uids.
 */
int uid_readDocumentUIDs(TixiDocument* tixiDocument);

/**
 * Builds an internal structure of all uids. Memory allocation etc.
 */
int uid_buildUIDNodeList(TixiDocument* tixiDocument, xmlNodeSetPtr nodes);

/**
 * Checks for not-unique uIDs.
 */
int uid_checkForDuplicates(TixiDocument *document);

/**
 * Checks for broken links (all links without a corrsponding UID).
 */
int uid_checkForBrokenLinks(TixiDocument *document);

/**
 * Cleans up memory and removed the uid-list from the document.
 */
int uid_clearUIDList(TixiDocument *document);

/**
 * Returns a XPath to a given uID.
 */
char* uid_getXpath(TixiDocument *document, char *uID);

/**
 * Checks if a UID exists.
 */
int uid_checkExists(TixiDocument *document, char *uID);

