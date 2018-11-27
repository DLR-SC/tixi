/*
* Copyright (C) 2016 German Aerospace Center (DLR/SC)
*
* Created: 2016-08-23 Martin Siggel <Martin.Siggel@dlr.de>
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*   http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#ifndef TIXI_NAMESPACEFUNCTIONS_H
#define TIXI_NAMESPACEFUNCTIONS_H

#include <libxml/tree.h>
#include "tixiInternal.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief extractPrefixAndName Tokenizes the string into prefix and element name
 *
 * The user must free the memory after this call.
 */
TIXI_INTERNAL_EXPORT void extractPrefixAndName(const char* qualifiedName, char** prefix, char** name);

/**
 * @brief Searches in the given node and its parent nodes for a
 * namespace definition defined by its URI.
 *
 * @return
 *   The namespace, if it was found, otherwise a null pointer.
 */
TIXI_INTERNAL_EXPORT xmlNsPtr findNamespace(const xmlDocPtr document, const xmlNodePtr node, const char* namespaceURI);

/**
 * @brief nodeSetNamespace Sets the namespace for a node
 *
 * The prefix can also be empty / null.
 * @return
 */
TIXI_INTERNAL_EXPORT xmlNsPtr nodeSetNamespace(const xmlDocPtr doc, xmlNodePtr node, const char* prefix, const char* namespaceURI);

/**
 * @brief Adds the namespace attribute to a node.
 *
 * The prefix must not be empty!
 * @return
 */
TIXI_INTERNAL_EXPORT xmlNsPtr nodeAddNamespace(const xmlDocPtr doc, xmlNodePtr node, const char* prefix, const char* namespaceURI);

#ifdef __cplusplus
} /* extern C */
#endif

#endif // TIXI_NAMESPACEFUNCTIONS_H

