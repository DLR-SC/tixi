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

xmlXPathObjectPtr XPathEvaluateExpression(xmlXPathContextPtr xpathContext, const char* xPathExpression)
{
  xmlXPathObjectPtr xpathObject;

  if (!xpathContext) {
    return NULL;
  }

  /* Evaluate Expression */
  xpathObject = xmlXPathEvalExpression((xmlChar*) xPathExpression, xpathContext);
  if (!(xpathObject)) {
    printMsg(MESSAGETYPE_ERROR, "Error: Invalid XPath expression \"%s\"\n", xPathExpression);
    return NULL;
  }

  if (xmlXPathNodeSetIsEmpty(xpathObject->nodesetval)) {
    xmlXPathFreeObject(xpathObject);
    return NULL;
  }

  return xpathObject;
}



int XPathGetNodeNumber(TixiDocument* tixiDocument, const char* xPathExpression)
{
  xmlXPathObjectPtr xpathObject;
  xmlNodeSetPtr nodes = NULL;
  int nodeNr = 0;

  /* Load XML document */
  xpathObject = XPathEvaluateExpression(tixiDocument->xpathContext, xPathExpression);
  if (xpathObject == NULL) {
    return -1;
  }

  nodes = xpathObject->nodesetval;
  nodeNr = nodes->nodeNr;
  xmlXPathFreeObject(xpathObject);
  return nodeNr;
}



char* XPathExpressionGetText(TixiDocument* tixiDocument, const char* xPathExpression, int index)
{

  xmlXPathObjectPtr xpathObject;
  xmlNodeSetPtr nodes = NULL;
  xmlNodePtr cur;
  char* text = NULL;
  int size = 0;

  xpathObject = XPathEvaluateExpression(tixiDocument->xpathContext, xPathExpression);
  if (xpathObject == NULL) {
    return NULL;
  }

  nodes = xpathObject->nodesetval;

  size = (nodes) ? nodes->nodeNr : 0;
  if (size == 0) {
    printMsg(MESSAGETYPE_ERROR, "Error: XPath Expression '%s' returns 0 nodes.\n", xPathExpression);
    xmlXPathFreeObject(xpathObject);
    return NULL;
  }

  if (size < index) {
    printMsg(MESSAGETYPE_ERROR, "Error: Index number too high, XPath expression only returns %d nodes.\n", size);
    xmlXPathFreeObject(xpathObject);
    return NULL;
  }

  if (index <= 0) {
    printMsg(MESSAGETYPE_ERROR, "Error: Index number less or equal zero.\n");
    xmlXPathFreeObject(xpathObject);
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
      xmlXPathFreeObject(xpathObject);
      return NULL;
    }
  }

  xmlXPathFreeObject(xpathObject);
  return text;
}

char* XPathExpressionGetElementName(TixiDocument* tixiDocument, const char* xPathExpression, int index)
{

  xmlXPathObjectPtr xpathObject;
  xmlNodeSetPtr nodes = NULL;
  xmlNodePtr cur;
  int size = 0;

  xpathObject = XPathEvaluateExpression(tixiDocument->xpathContext, xPathExpression);
  if (xpathObject == NULL) {
    return NULL;
  }

  nodes = xpathObject->nodesetval;

  size = (nodes) ? nodes->nodeNr : 0;
  if (size == 0) {
    printMsg(MESSAGETYPE_ERROR, "Error: XPath Expression '%s' returns 0 nodes.\n", xPathExpression);
    xmlXPathFreeObject(xpathObject);
    return NULL;
  }

  if (size < index) {
    printMsg(MESSAGETYPE_ERROR, "Error: Index number too high, XPath expression only returns %d nodes.\n", size);
    xmlXPathFreeObject(xpathObject);
    return NULL;
  }

  if (index <= 0) {
    printMsg(MESSAGETYPE_ERROR, "Error: Index number less or equal zero.\n");
    xmlXPathFreeObject(xpathObject);
    return NULL;
  }

  cur = nodes->nodeTab[--index];

  if (cur->type == XML_ELEMENT_NODE) {
    xmlXPathFreeObject(xpathObject);
    return (char*)cur->name;
  }
  else if (cur->type == XML_ATTRIBUTE_NODE) {
    xmlXPathFreeObject(xpathObject);
    return NULL;
  }
  xmlXPathFreeObject(xpathObject);
  return NULL;
}

char* XPathExpressionGetElementPath(TixiDocument* tixiDocument, const char* xPathExpression, int index)
{

  xmlXPathObjectPtr xpathObject;
  xmlNodeSetPtr nodes = NULL;
  xmlNodePtr cur;
  int size = 0;

  xpathObject = XPathEvaluateExpression(tixiDocument->xpathContext, xPathExpression);
  if (xpathObject == NULL) {
    return NULL;
  }

  nodes = xpathObject->nodesetval;

  size = (nodes) ? nodes->nodeNr : 0;
  if (size == 0) {
    printMsg(MESSAGETYPE_ERROR, "Error: XPath Expression '%s' returns 0 nodes.\n", xPathExpression);
    xmlXPathFreeObject(xpathObject);
    return NULL;
  }

  if (size < index) {
    printMsg(MESSAGETYPE_ERROR, "Error: Index number too high, XPath expression only returns %d nodes.\n", size);
    xmlXPathFreeObject(xpathObject);
    return NULL;
  }

  if (index <= 0) {
    printMsg(MESSAGETYPE_ERROR, "Error: Index number less or equal zero.\n");
    xmlXPathFreeObject(xpathObject);
    return NULL;
  }

  cur = nodes->nodeTab[--index];
  xmlXPathFreeObject(xpathObject);

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
