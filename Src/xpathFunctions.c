/* 
* Copyright (C) 2007-2011 German Aerospace Center (DLR/SC)
*
* Created: 2010-08-13 Markus Litz <Markus.Litz@dlr.de>
* Changed: $Id: dbms.h 4577 20xx-xx-xx 09:27:39Z litz_ma $ 
*
* Version: $Revision: 3978 $
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
#include "xpathFunctions.h"
#include "tixiInternal.h"


xmlXPathObjectPtr XPathEvaluateExpression(xmlDocPtr doc, char *xPathExpression)
{
	xmlXPathContextPtr xpathContext;
	xmlXPathObjectPtr xpathObject;

	/* Create xpath evaluation context */
	xpathContext = xmlXPathNewContext(doc);
	if (!xpathContext) {
	  fprintf(stderr, "Error: Unable to create new XPath context.\n");
	  return NULL;
	}

	/* Evaluate Expression */
	xpathObject = xmlXPathEvalExpression((xmlChar *) xPathExpression, xpathContext);
	if (!(xpathObject)) {
	  fprintf(stderr, "Error: Invalid XPath expression \"%s\"\n", xPathExpression);
	  xmlXPathFreeContext(xpathContext);
	  return NULL;
	}

	if (xmlXPathNodeSetIsEmpty(xpathObject->nodesetval)) {
	  xmlXPathFreeContext(xpathContext);
	  xmlXPathFreeObject(xpathObject);
	  return NULL;
	}

	return xpathObject;
}



int XPathGetNodeNumber(TixiDocument *tixiDocument, char *xPathExpression)
{
	xmlDocPtr doc;
    xmlXPathObjectPtr xpathObject;
    xmlNodeSetPtr nodes = NULL;

    /* Load XML document */
    doc = tixiDocument->docPtr;
    xpathObject = XPathEvaluateExpression(doc, xPathExpression);
    if (xpathObject == NULL) {
    	return -1;
    }

    nodes = xpathObject->nodesetval;
    return nodes->nodeNr;
}



char* XPathExpressionGetText(TixiDocument *tixiDocument, char *xPathExpression, int index)
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
		fprintf(stderr, "Error: XPath Expression '%s' returns 0 nodes.\n", xPathExpression);
		return NULL;
	}

	if (size < index) {
		fprintf(stderr, "Error: Index number too high, XPath expression only returns %d nodes.\n", size);
		return NULL;
	}

	if (index <= 0) {
		fprintf(stderr, "Error: Index number less or equal zero.\n");
		return NULL;
	}

	cur = nodes->nodeTab[--index];

	if (cur->type == XML_ELEMENT_NODE) {
		xmlNodePtr children = cur->children;
		text = (char *) xmlNodeListGetString(doc, children, 0);
	} else if (cur->type == XML_ATTRIBUTE_NODE) {
		if (cur->children) {
			text = cur->children->content;
		} else {
			return NULL;
		}
	}

	return text;
}

