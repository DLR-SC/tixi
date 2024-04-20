/*
* Copyright (C) 20015 German Aerospace Center (DLR/SC)
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
#include "uidHelper.h"
#include "tixiInternal.h"
#include <stdlib.h>

extern void printMsg(MessageType type, const char* message, ...);

int uid_readDocumentUIDs(TixiDocument* tixiDocument)
{
  xmlXPathObjectPtr xpathObj;

  /* Evaluate xpath expression */
  xpathObj = xmlXPathEvalExpression((xmlChar*) CPACS_UID_XPATH, tixiDocument->xpathContext);
  if (xpathObj == NULL) {
    printMsg(MESSAGETYPE_ERROR,"Error: unable to evaluate xpath expression \"%s\"\n", CPACS_UID_XPATH);
    return(FAILED);
  }

  /* Build List of results */
  if (uid_buildUIDNodeList(tixiDocument, xpathObj->nodesetval) != SUCCESS) {
    return FAILED;
  }

  /* Cleanup */
  xmlXPathFreeObject(xpathObj);

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
    free(tixiDocument->uidListHead);
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
      printMsg(MESSAGETYPE_ERROR, "Error: Memory allocation failed in uidHelper::buildUIDNodeList");
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
  while (currentEntry) {
    if (currentEntry->next) {
      compareElement = document->uidListHead;
      while (compareElement) {
        if (compareElement == currentEntry) {
          compareElement = (TixiUIDListEntry *) compareElement->next;
          continue;
        }
        if (!strcmp("", currentEntry->uIDName)) {
          // if we found an emtpy uid, we only warn about that.
          if (alreadyDisplayed) {
            printMsg(MESSAGETYPE_WARNING, "Warning: Empty uID found! This might lead to unknown errors!\n");
          }
          alreadyDisplayed=0;
        }
        else if (!strcmp(compareElement->uIDName, currentEntry->uIDName)) {
          printMsg(MESSAGETYPE_ERROR, "Error: duplicated uID '%s' found!\n", currentEntry->uIDName);
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
  xmlXPathObjectPtr xpathObj;
  xmlNodePtr cur;
  int size;
  int i;
  char *linkName;
  xmlNodeSetPtr nodes;
  ReturnCode foundUID = FAILED;

  /* Evaluate xpath expression */
  xpathObj = xmlXPathEvalExpression((xmlChar*) CPACS_UID_LINK_XPATH, document->xpathContext);
  if (xpathObj == NULL) {
    printMsg(MESSAGETYPE_ERROR,"Error: unable to evaluate xpath expression \"%s\"\n", CPACS_UID_LINK_XPATH);
    return(FAILED);
  }

  /* Build List of results */
  nodes = xpathObj->nodesetval;
  size = (nodes) ? nodes->nodeNr : 0;

  if (size == 0) {
    xmlXPathFreeObject(xpathObj);
    return SUCCESS;
  }

  // iterate through all links
  for (i = 0; i < size; ++i) {
    cur = nodes->nodeTab[i];
    linkName = (char*) xmlGetProp(cur->parent, (xmlChar*) CPACS_UID_LINK_ATTRIBUTE_STRING);

    // now check if a corresponding uid exists
    currentEntry = document->uidListHead;
    foundUID = FAILED;
    while (currentEntry) {
      if (!strcmp(linkName, currentEntry->uIDName)) {
        foundUID = SUCCESS;
        break;
      }
      currentEntry = (TixiUIDListEntry *) currentEntry->next;
    }
    if (foundUID == FAILED) {
      printMsg(MESSAGETYPE_ERROR, "Error: Broken link, UID '%s' not found!", linkName);
      xmlXPathFreeObject(xpathObj);
      xmlFree(linkName);
      return UID_LINK_BROKEN;
    }
    xmlFree(linkName);
  }

  /* Cleanup */
  xmlXPathFreeObject(xpathObj);

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


char* uid_getXpath(TixiDocument *document, const char *uID)
{
  TixiUIDListEntry *current = document->uidListHead;

  while (current) {
    if (!strcmp(current->uIDName, uID)) {
      return (char*) xmlGetNodePath(current->nodePtr);
    }
    current = (TixiUIDListEntry *) current->next;
  }
  return NULL;
}


int uid_checkExists(TixiDocument *document, const char *uID)
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

