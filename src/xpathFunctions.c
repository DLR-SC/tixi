/*
* Copyright (C) 2015 German Aerospace Center (DLR/SC)
*
* Created: 2010-08-13 Markus Litz <Markus.Litz@dlr.de>
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
#include "xpathFunctions.h"
#include "tixiInternal.h"

#include "libxml/xpathInternals.h"

#include <assert.h>

extern void printMsg(MessageType type, const char* message, ...);

xmlXPathObjectPtr XPathEvaluateExpression(TixiDocument* doc, const char* xPathExpression)
{
  xmlXPathObjectPtr xpathObject;

  if (!doc || !doc->xpathContext) {
    return NULL;
  }
  
  xpathObject = XPathCacheGet(doc->xpathCache,(const xmlChar*) xPathExpression);
  if (xpathObject) {
    return xpathObject;
  }

  /* Evaluate Expression */
  xpathObject = xmlXPathEvalExpression((const xmlChar*) xPathExpression, doc->xpathContext);
  if (!(xpathObject)) {
    printMsg(MESSAGETYPE_ERROR, "Error: Invalid XPath expression \"%s\"\n", xPathExpression);
    return NULL;
  }

  XPathCacheInsert(doc->xpathCache, (const xmlChar*) xPathExpression, xpathObject);

  return xpathObject;
}



int XPathGetNodeNumber(TixiDocument* tixiDocument, const char* xPathExpression)
{
  xmlXPathObjectPtr xpathObject;
  xmlNodeSetPtr nodes = NULL;
  int nodeNr = 0;

  /* Load XML document */
  xpathObject = XPathEvaluateExpression(tixiDocument, xPathExpression);
  if (xpathObject == NULL) {
    return -1;
  }

  if (xmlXPathNodeSetIsEmpty(xpathObject->nodesetval)) {
      return 0;
  }

  nodes = xpathObject->nodesetval;
  nodeNr = nodes->nodeNr;
  return nodeNr;
}



char* XPathExpressionGetText(TixiDocument* tixiDocument, const char* xPathExpression, int index)
{

  xmlXPathObjectPtr xpathObject;
  xmlNodeSetPtr nodes = NULL;
  xmlNodePtr cur;
  char* text = NULL;
  int size = 0;

  xpathObject = XPathEvaluateExpression(tixiDocument, xPathExpression);
  if (xpathObject == NULL) {
    return NULL;
  }

  nodes = xpathObject->nodesetval;

  size = (nodes) ? nodes->nodeNr : 0;
  if (size == 0) {
    printMsg(MESSAGETYPE_ERROR, "Error: XPath Expression '%s' returns 0 nodes.\n", xPathExpression);
    return NULL;
  }

  if (size < index) {
    printMsg(MESSAGETYPE_ERROR, "Error: Index number too high, XPath expression only returns %d nodes.\n", size);
    return NULL;
  }

  if (index <= 0) {
    printMsg(MESSAGETYPE_ERROR, "Error: Index number less or equal zero.\n");
    return NULL;
  }

  cur = nodes->nodeTab[--index];

  if (cur->type == XML_ELEMENT_NODE) {
    xmlNodePtr children = cur->children;
    text = (char*) xmlNodeListGetString(tixiDocument->docPtr, children, 0);
  }
  else if (cur->type == XML_ATTRIBUTE_NODE) {
    if (cur->children) {
      text = (char*) cur->children->content;
    }
    else {
      return NULL;
    }
  }

  return text;
}

char* XPathExpressionGetElementName(TixiDocument* tixiDocument, const char* xPathExpression, int index)
{

  xmlXPathObjectPtr xpathObject;
  xmlNodeSetPtr nodes = NULL;
  xmlNodePtr cur;
  int size = 0;

  xpathObject = XPathEvaluateExpression(tixiDocument, xPathExpression);
  if (xpathObject == NULL) {
    return NULL;
  }

  nodes = xpathObject->nodesetval;

  size = (nodes) ? nodes->nodeNr : 0;
  if (size == 0) {
    printMsg(MESSAGETYPE_ERROR, "Error: XPath Expression '%s' returns 0 nodes.\n", xPathExpression);
    return NULL;
  }

  if (size < index) {
    printMsg(MESSAGETYPE_ERROR, "Error: Index number too high, XPath expression only returns %d nodes.\n", size);
    return NULL;
  }

  if (index <= 0) {
    printMsg(MESSAGETYPE_ERROR, "Error: Index number less or equal zero.\n");
    return NULL;
  }

  cur = nodes->nodeTab[--index];

  if (cur->type == XML_ELEMENT_NODE) {
    return (char*)cur->name;
  }
  else if (cur->type == XML_ATTRIBUTE_NODE) {
    return NULL;
  }
  return NULL;
}

char* XPathExpressionGetElementPath(TixiDocument* tixiDocument, const char* xPathExpression, int index)
{

  xmlXPathObjectPtr xpathObject;
  xmlNodeSetPtr nodes = NULL;
  xmlNodePtr cur;
  int size = 0;

  xpathObject = XPathEvaluateExpression(tixiDocument, xPathExpression);
  if (xpathObject == NULL) {
    return NULL;
  }

  nodes = xpathObject->nodesetval;

  size = (nodes) ? nodes->nodeNr : 0;
  if (size == 0) {
    printMsg(MESSAGETYPE_ERROR, "Error: XPath Expression '%s' returns 0 nodes.\n", xPathExpression);
    return NULL;
  }

  if (size < index) {
    printMsg(MESSAGETYPE_ERROR, "Error: Index number too high, XPath expression only returns %d nodes.\n", size);
    return NULL;
  }

  if (index <= 0) {
    printMsg(MESSAGETYPE_ERROR, "Error: Index number less or equal zero.\n");
    return NULL;
  }

  cur = nodes->nodeTab[--index];

  return (char*) xmlGetNodePath(cur);
}

int XPathRegisterNamespace(xmlXPathContextPtr xpathContext, const char *namespaceURI, const char *prefix)
{
  return xmlXPathRegisterNs(xpathContext, (xmlChar*) prefix, (xmlChar*) namespaceURI);
}

int XPathRegisterDocumentNamespaces(xmlXPathContextPtr xpathContext)
{
  xmlXPathObjectPtr xpathObj;
  xmlNodeSetPtr nodes;
  int nodeCount = 0;
  int inode = 0;
  int error = 0;


  /* Get all unique namespace declarations */
  xpathObj = xmlXPathEvalExpression((xmlChar*) "//*/namespace::*[not(. = ../../namespace::*|preceding::*/namespace::*)]", xpathContext);
  if (xpathObj == NULL) {
    printMsg(MESSAGETYPE_ERROR, "Error: unable to retrieve all namespaces \n");
    return -1;
  }

  nodes = xpathObj->nodesetval;
  nodeCount = (nodes) ? nodes->nodeNr : 0;

  for (inode = 0; inode < nodeCount; ++inode) {

    if (nodes->nodeTab[inode] && nodes->nodeTab[inode]->type == XML_NAMESPACE_DECL) {
      xmlNsPtr ns = (xmlNsPtr)nodes->nodeTab[inode];

      if (!ns) {
        continue;
      }

      /* ignore default xml prefix */
      if (ns->prefix && strcmp((char*)ns->prefix, "xml") != 0 ) {
        if (XPathRegisterNamespace(xpathContext, (char*)ns->href, (char*) ns->prefix) != 0) {
          printMsg(MESSAGETYPE_ERROR, "Error: unable to register NS with prefix=\"%s\" and href=\"%s\"\n", ns->prefix, ns->href);
          error = -1;
        }
      }
      else if(!ns->prefix) {
        printMsg(MESSAGETYPE_STATUS, "Skipping default namespace \"%s\"\n", ns->href);
      }

    } /* is namespace */
  } /* for */

  xmlXPathFreeObject(xpathObj);

  return error;
}

XPathCache *XPathNewCache()
{
    XPathCache* cache = (XPathCache*) malloc(sizeof(XPathCache));

    cache->result = 0;
    cache->xpath = 0;
    cache->enabled = 0;

    return cache;
}

void XPathClearCache(XPathCache * cache)
{
    if(cache->result) xmlXPathFreeObject(cache->result);
    if(cache->xpath) xmlFree(cache->xpath);

    cache->result = 0;
    cache->xpath = 0;
}

void XPathFreeCache(XPathCache * cache)
{
    XPathClearCache(cache);
    free(cache);
}

xmlXPathObjectPtr XPathCacheGet(XPathCache const * cache, xmlChar const * xpath)
{
    if (!cache || cache->enabled != 1) {
        return NULL;
    }

    if (!xpath || !cache->xpath || xmlStrcmp(cache->xpath, xpath) != 0) {
        return NULL;
    }

    return cache->result;
}

void XPathCacheInsert(XPathCache *cache, const xmlChar *xpath, const xmlXPathObjectPtr result)
{
    if (!cache || !xpath || !result) {
        return;
    }

    XPathClearCache(cache);

    cache->result = result;
    cache->xpath = (xmlChar*) malloc((strlen((const char*)xpath) + 3) * sizeof (xmlChar*));
    strcpy((char*) cache->xpath, (const char*) xpath);
}
