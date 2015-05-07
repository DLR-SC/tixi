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

/**
 * @file   xpathFunctions.h
 * @author Markus Litz <Markus.Litz@dlr.de>
 * @date   Tue Jul 25 12:06:33 2010
 * @brief  Header file for managing xpath expressions.
 */

#ifndef XPATH_FUNCTIONS_H
#define XPATH_FUNCTIONS_H

#include "libxml/parser.h"
#include "libxml/xpath.h"
#include "libxml/xmlsave.h"
#include "tixi.h"
#include "tixiData.h"

#ifdef __cplusplus
extern "C" {
#endif

xmlXPathObjectPtr XPathEvaluateExpression(xmlDocPtr document, const char *xPathExpression);

int XPathGetNodeNumber(TixiDocument *tixiDocument, const char *xPathExpression);

char* XPathExpressionGetText(TixiDocument *tixiDocument, const char *xPathExpression, int index);

/**
  @brief XPathExpressionGetElementPath Returns the name of the i-th element matching the xPathExpression

  @return
    Name of the element. Null in case if an error. The String must not be freed by the user.
 */
char* XPathExpressionGetElementName(TixiDocument *tixiDocument, const char *xPathExpression, int index);

/**
  @brief XPathExpressionGetElementPath Returns the XPath to the i-th element matching the xPathExpressio

  @return
    XPath String. Null in case if an error.
 */
char* XPathExpressionGetElementPath(TixiDocument *tixiDocument, const char *xPathExpression, int index);

#ifdef __cplusplus
}
#endif

#endif /* XPATH_FUNCTIONS_H */
