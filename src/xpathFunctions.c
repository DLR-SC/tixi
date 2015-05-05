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
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
#include "xpathFunctions.h"
#include "tixiInternal.h"

extern TixiPrintMsgFnc printMsg;

xmlXPathObjectPtr XPathEvaluateExpression(xmlDocPtr doc, const char *xPathExpression)
{
	xmlXPathContextPtr xpathContext;
	xmlXPathObjectPtr xpathObject;

	/* Create xpath evaluation context */
	xpathContext = xmlXPathNewContext(doc);
	if (!xpathContext) {
	  printMsg(MESSAGETYPE_ERROR, "Error: Unable to create new XPath context.\n");
	  return NULL;
	}

	/* Evaluate Expression */
	xpathObject = xmlXPathEvalExpression((xmlChar *) xPathExpression, xpathContext);
	if (!(xpathObject)) {
	  printMsg(MESSAGETYPE_ERROR, "Error: Invalid XPath expression \"%s\"\n", xPathExpression);
	  xmlXPathFreeContext(xpathContext);
	  return NULL;
	}

	if (xmlXPathNodeSetIsEmpty(xpathObject->nodesetval)) {
	  xmlXPathFreeContext(xpathContext);
	  xmlXPathFreeObject(xpathObject);
	  return NULL;
	}

	xmlXPathFreeContext(xpathContext);
	return xpathObject;
}



int XPathGetNodeNumber(TixiDocument *tixiDocument, const char *xPathExpression)
{
	xmlDocPtr doc;
    xmlXPathObjectPtr xpathObject;
    xmlNodeSetPtr nodes = NULL;
    int nodeNr = 0;

    /* Load XML document */
    doc = tixiDocument->docPtr;
    xpathObject = XPathEvaluateExpression(doc, xPathExpression);
    if (xpathObject == NULL) {
    	return -1;
    }

    nodes = xpathObject->nodesetval;
    nodeNr = nodes->nodeNr;
    xmlXPathFreeObject(xpathObject);
    return nodeNr;
}



char* XPathExpressionGetText(TixiDocument *tixiDocument, const char *xPathExpression, int index)
{

	xmlDocPtr doc;
    xmlXPathObjectPtr xpathObject;
    xmlNodeSetPtr nodes = NULL;
    xmlNodePtr cur;
	char* text = NULL;
	int size = 0;

	/* Load XML document */
	doc = tixiDocument->docPtr;

	xpathObject = XPathEvaluateExpression(doc, xPathExpression);
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
		text = (char *) xmlNodeListGetString(doc, children, 0);
	} else if (cur->type == XML_ATTRIBUTE_NODE) {
		if (cur->children) {
			text = (char*) cur->children->content;
		} else {
			xmlXPathFreeObject(xpathObject);
			return NULL;
		}
	}

	xmlXPathFreeObject(xpathObject);
	return text;
}

char* XPathExpressionGetElementName(TixiDocument *tixiDocument, const char *xPathExpression, int index)
{

    xmlDocPtr doc;
    xmlXPathObjectPtr xpathObject;
    xmlNodeSetPtr nodes = NULL;
    xmlNodePtr cur;
    int size = 0;

    /* Load XML document */
    doc = tixiDocument->docPtr;

    xpathObject = XPathEvaluateExpression(doc, xPathExpression);
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
    } else if (cur->type == XML_ATTRIBUTE_NODE) {
        xmlXPathFreeObject(xpathObject);
        return NULL;
    }
    xmlXPathFreeObject(xpathObject);
    return NULL;
}

char* XPathExpressionGetElementPath(TixiDocument *tixiDocument, const char *xPathExpression, int index)
{

    xmlDocPtr doc;
    xmlXPathObjectPtr xpathObject;
    xmlNodeSetPtr nodes = NULL;
    xmlNodePtr cur;
    int size = 0;

    /* Load XML document */
    doc = tixiDocument->docPtr;

    xpathObject = XPathEvaluateExpression(doc, xPathExpression);
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

