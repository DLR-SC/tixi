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
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
#define _CRT_SECURE_NO_WARNINGS
#include "uidHelper.h"
#include "tixiInternal.h"



int uid_readDocumentUIDs(TixiDocument* tixiDocument)
{
	xmlDocPtr doc;
	xmlXPathContextPtr xpathCtx;
	xmlXPathObjectPtr xpathObj;

	/* Load XML document */
	doc = tixiDocument->docPtr;

	/* Create xpath evaluation context */
	xpathCtx = xmlXPathNewContext(doc);
	if(xpathCtx == NULL) {
		fprintf(stderr,"Error: unable to create new XPath context\n");
		xmlFreeDoc(doc);
		return(FAILED);
	}

	/* Evaluate xpath expression */
	xpathObj = xmlXPathEvalExpression(CPACS_UID_XPATH, xpathCtx);
	if(xpathObj == NULL) {
		fprintf(stderr,"Error: unable to evaluate xpath expression \"%s\"\n", CPACS_UID_XPATH);
		xmlXPathFreeContext(xpathCtx);
		xmlFreeDoc(doc);
		return(FAILED);
	}

	/* Build List of results */
	if (uid_buildUIDNodeList(tixiDocument, xpathObj->nodesetval) != SUCCESS) {
		return FAILED;
	}

	/* Cleanup */
	xmlXPathFreeObject(xpathObj);
	xmlXPathFreeContext(xpathCtx);

	return(SUCCESS);
}



int uid_buildUIDNodeList(TixiDocument* tixiDocument, xmlNodeSetPtr nodes)
{
    xmlNodePtr cur;
    int size;
    int i;
    TixiUIDListEntry* currentEntry;
    size = (nodes) ? nodes->nodeNr : 0;

    tixiDocument->uidListHead = (TixiUIDListEntry *) malloc(sizeof(TixiUIDListEntry));
    currentEntry = tixiDocument->uidListHead;

	if (nodes->nodeNr == 0) {
		tixiDocument->uidListHead = NULL;
		return SUCCESS;
	}

    cur = nodes->nodeTab[0];
    currentEntry->nodePtr = cur->parent;
    currentEntry->uIDName = (char *) xmlGetProp(cur->parent, (xmlChar *) CPACS_UID_ATTRIBUTE_STRING);
	currentEntry->next = NULL;

    for (i = 1; i < size; ++i) {
		cur = nodes->nodeTab[i];

		// add new uid entry
		currentEntry->next = malloc(sizeof(TixiUIDListEntry));
		if (!currentEntry) {
			fprintf(stderr, "Error: Memory allocation failed in uidHelper::buildUIDNodeList");
			return MEMORY_ALLOCATION_FAILED;
		}
		currentEntry = (TixiUIDListEntry *) currentEntry->next;
		currentEntry->nodePtr = cur->parent;
		currentEntry->uIDName = (char *) xmlGetProp(cur->parent, (xmlChar *) CPACS_UID_ATTRIBUTE_STRING);
		currentEntry->next = NULL;
    }

    return SUCCESS;
}

int uid_checkForDuplicates(TixiDocument *document)
{
	int alreadyDisplayed = -1;
	TixiUIDListEntry* currentEntry;
	TixiUIDListEntry* compareElement;

	currentEntry = document->uidListHead;
	while(currentEntry) {
		if (currentEntry->next) {
			compareElement = document->uidListHead;
			while(compareElement) {
				if (compareElement == currentEntry) {
					compareElement = (TixiUIDListEntry *) compareElement->next;
					continue;
				} if (!strcmp("", currentEntry->uIDName)) {
					// if we found an emtpy uid, we only warn about that.
					if (alreadyDisplayed) fprintf(stderr, "Warning: Empty uID found! This might lead to unknown errors!\n");
					alreadyDisplayed=0; 
				} else if (!strcmp(compareElement->uIDName, currentEntry->uIDName)) {
					fprintf(stderr, "Error: duplicated uID '%s' found!\n", currentEntry->uIDName);
					return FAILED;
				}
				compareElement = (TixiUIDListEntry *) compareElement->next;
			}
		}
		currentEntry = (TixiUIDListEntry *) currentEntry->next;
	}
	return SUCCESS;
}



int uid_checkForBrokenLinks(TixiDocument *document)
{
	TixiUIDListEntry* currentEntry;
	TixiUIDListEntry* compareElement;
	xmlDocPtr doc;
	xmlXPathContextPtr xpathCtx;
	xmlXPathObjectPtr xpathObj;
	xmlNodePtr cur;
	int size;
	int i;
	char *linkName;
	xmlNodeSetPtr nodes;
	ReturnCode foundUID = FAILED;

	/* Load XML document */
	doc = document->docPtr;

	/* Create xpath evaluation context */
	xpathCtx = xmlXPathNewContext(doc);
	if(xpathCtx == NULL) {
		fprintf(stderr,"Error: unable to create new XPath context\n");
		xmlFreeDoc(doc);
		return(FAILED);
	}

	/* Evaluate xpath expression */
	xpathObj = xmlXPathEvalExpression(CPACS_UID_LINK_XPATH, xpathCtx);
	if(xpathObj == NULL) {
		fprintf(stderr,"Error: unable to evaluate xpath expression \"%s\"\n", CPACS_UID_LINK_XPATH);
		xmlXPathFreeContext(xpathCtx);
		xmlFreeDoc(doc);
		return(FAILED);
	}

	/* Build List of results */
	nodes = xpathObj->nodesetval;
	size = (nodes) ? nodes->nodeNr : 0;

	if (size == 0) {
		return SUCCESS;
	}

	// iterate through all links
	for (i = 0; i < size; ++i) {
		cur = nodes->nodeTab[i];
		linkName = xmlGetProp(cur->parent, CPACS_UID_LINK_ATTRIBUTE_STRING);

		// now check if a corresponding uid exists
		currentEntry = document->uidListHead;
		foundUID = FAILED;
		while(currentEntry) {
			if (!strcmp(linkName, currentEntry->uIDName)) {
				xmlXPathFreeObject(xpathObj);
				xmlXPathFreeContext(xpathCtx);
				foundUID = SUCCESS;
				break;
			}
			currentEntry = (TixiUIDListEntry *) currentEntry->next;
		}
		if (foundUID == FAILED) {
			fprintf(stderr, "Error: Broken link, UID '%s' not found!", linkName);
			xmlXPathFreeObject(xpathObj);
			xmlXPathFreeContext(xpathCtx);
			return UID_LINK_BROKEN;
		}
		xmlFree(linkName);
	}

	/* Cleanup */
	xmlXPathFreeObject(xpathObj);
	xmlXPathFreeContext(xpathCtx);

	return SUCCESS;
}



int uid_clearUIDList(TixiDocument *document)
{
	TixiUIDListEntry *current = document->uidListHead;
	while (current) {
		TixiUIDListEntry *next = (TixiUIDListEntry *) current->next;
		if (current->uIDName) {
			xmlFree(current->uIDName);
			current->uIDName = NULL;
		}
		free(current);
		current = next;
	}
	document->uidListHead = NULL;
	return SUCCESS;
}


char* uid_getXpath(TixiDocument *document, char *uID)
{
	TixiUIDListEntry *current = document->uidListHead;

	while (current) {
		if (!strcmp(current->uIDName, uID)) {
			return xmlGetNodePath(current->nodePtr);
		}
		current = (TixiUIDListEntry *) current->next;
	}
	return NULL;
}


int uid_checkExists(TixiDocument *document, char *uID)
{
	TixiUIDListEntry *current = document->uidListHead;

	while (current) {
		if (!strcmp(current->uIDName, uID)) {
			return 0;
		}
		current = (TixiUIDListEntry *) current->next;
	}
	return -1;
}

