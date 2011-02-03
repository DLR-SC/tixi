/* 
* Copyright (C) 2007-2011 German Aerospace Center (DLR/SC)
*
* Created: 2010-08-13 Markus Litz <Markus.Litz@dlr.de>
* Changed: $Id: xslTransformation.h 4392 2011-02-03 08:52:55Z litz_ma $ 
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
 * @file   xslTransformation.h
 * @author Markus Litz <Markus.Litz@dlr.de>
 * @date   Tue May 28 12:06:33 2009
 * @brief Utility functions used to perform xsl transformations.
 * @file Utility functions used to perform xsl transformations.
 */
#include "libxml/parser.h"
#include "libxml/xpath.h"
#include "libxml/xmlsave.h"
#include "libxslt/xslt.h"
#include "libxslt/xsltInternals.h"
#include "libxslt/transform.h"
#include "libxslt/xsltutils.h"
#include "tixi.h"
#include "tixiData.h"


/**
    @brief Performs XSL transformation to a document and returns the result doc as string.

    @param xmlDocPtr doc (in) The document on which the transformation should be performed
    @param char *xslFilename (in) Filename of the stylesheet file
    @return char* The resulting document as string
 */
char* xsltTransformToString(xmlDocPtr doc, char *xslFilename);

/**
    @brief Performs XSL transformation to a document and saves the resulting doc in a local file.

    @param xmlDocPtr doc (in) The document on which the transformation should be performed
    @param char *xslFilename (in) Filename of the stylesheet file
    @param char *outputFilename (in) Filename of the file in whoch the result should be saved
    @return ReturnCode
     - SUCCESS              if the document is successfully validated
     - FAILED                  for all internal errors
 */
ReturnCode xsltTransformToFile(xmlDocPtr doc, char *xslFilename, char *outputFilename);
