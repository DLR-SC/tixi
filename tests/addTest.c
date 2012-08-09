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
/**
@file	Implementation of addTest.
@brief addTestSuite add tests suites to the framework
*/
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "CUnit/CUnit.h"
#include "testfunctions.h"

#define PRINT_ERROR { fprintf(stderr, "At line %d in file %s:\n %s\n", __LINE__, __FILE__, CU_get_error_msg() ); exit(1);}

void addTests(void)
{

  CU_pSuite pSuite;

  assert(CU_get_registry() != NULL);
  assert(!CU_is_test_running());

  /* test for internal functions */

  printf("Add suite for testing the function from tixiInternal.c .\n");

  if (!(pSuite = CU_add_suite("internal", NULL, NULL)))
    PRINT_ERROR;
  if (!(CU_add_test(pSuite, "buildString_success", buildString_success)))
    PRINT_ERROR;

  printf("Add suite for tixiOpenDocument\n");

  if (!(pSuite = CU_add_suite("tixiOpenDocument", NULL, NULL)))
    PRINT_ERROR;

  if (!(CU_add_test(pSuite, "openNonExistingFile", openDocument_nonExisting)))
    PRINT_ERROR;
  if (!
      (CU_add_test
       (pSuite, "openNotWellFormedDocument", openDocument_notWellFormed)))
    PRINT_ERROR;
  if (!(CU_add_test(pSuite, "openIsWellFormed", openDocument_isWellFormed)))
    PRINT_ERROR;
  if (!(CU_add_test(pSuite, "openCheckHandle", openDocument_checkHandle)))
    PRINT_ERROR;
  if (!(CU_add_test(pSuite, "openHttpURL", openDocument_openHttpURL)))
    PRINT_ERROR;



  // ############################################
  printf("Add suite for tixiCloseDocument\n");

  if (!(pSuite = CU_add_suite("tixiCloseDocument", NULL, NULL)))
    PRINT_ERROR;

  if (!(CU_add_test(pSuite, "closeDocument", closeDocument_close)))
    PRINT_ERROR;
   
  if (!
      (CU_add_test
       (pSuite, "closeInvalidHandle", closeDocument_invalidHandle)))
    PRINT_ERROR;

  if (!(CU_add_test(pSuite, "closeDocument_closeMultiple", closeDocument_closeMultiple)))
    PRINT_ERROR;


  printf("Add suite for tixiGetXXXElement\n");

  if (!
      (pSuite =
       CU_add_suite("tixiGetElement", preGetElement, postGetElement)))
    PRINT_ERROR;

  if (!(CU_add_test(pSuite, "getText", getTextElement_getText)))
    PRINT_ERROR;
  if (!(CU_add_test(pSuite, "getEmptyElement", getEmptyElement)))
    PRINT_ERROR;
  if (!(CU_add_test(pSuite, "invalidHandle", getTextElement_invalidHandle)))
    PRINT_ERROR;
  if (!(CU_add_test(pSuite, "notUnique", getTextElement_notUnique)))
    PRINT_ERROR;
  if (!(CU_add_test(pSuite, "notAnElement", getTextElement_notAnElement)))
    PRINT_ERROR;
  if (!(CU_add_test(pSuite, "useIndexedPath", getTextElement_useIndexedPath)))
    PRINT_ERROR;
  if (!(CU_add_test(pSuite, "getDouble", getDoubleElement)))
    PRINT_ERROR;
  if (!(CU_add_test(pSuite, "getInteger", getIntegerElement)))
    PRINT_ERROR;  
  if (!(CU_add_test(pSuite, "getBoolean", getBooleanElement)))
    PRINT_ERROR;
  if (!(CU_add_test(pSuite, "getPoint", getPointElement)))
    PRINT_ERROR;
  if (!(CU_add_test(pSuite, "getFloatMatrix", getFloatMatrixElement)))
    PRINT_ERROR;
  if (!
      (CU_add_test
       (pSuite, "getMatrixOfPoints ", getElement_getMatrixOfPoints)))
    PRINT_ERROR;
  /* if ( !( CU_add_test( pSuite, "invalidXPath", getTextElement_invalidXPath )) ) PRINT_ERROR; */


    //################################################
    printf("Add suite for tixiRemoveElement\n");

  if (!
      (pSuite =
       CU_add_suite("tixiRemoveElement", preRemoveElement, postRemoveElement)))
    PRINT_ERROR;

  if (!(CU_add_test(pSuite, "getRemoveElement_singleElement", getRemoveElement_singleElement)))
    PRINT_ERROR;

   //################################################
    printf("Add suite for tixiRemoveAttribute\n");

  if (!
      (pSuite =
       CU_add_suite("tixiRemoveAttribute", preRemoveAttribute, postRemoveAttribute)))
    PRINT_ERROR;

  if (!(CU_add_test(pSuite, "getRemoveAttribute_singleAttribute", getRemoveAttribute_singleAttribute)))
    PRINT_ERROR;


  //################################################
/*    printf("Add suite for OpenExternalData\n");

  if (!
      (pSuite =
       CU_add_suite("openExternalData", preOpenExternalData, postOpenExternalData)))
    PRINT_ERROR;

  if (!(CU_add_test(pSuite, "openExternalData_getExternalData", openExternalData_getExternalData)))
    PRINT_ERROR;

    if (!(CU_add_test(pSuite, "openExternalData_writeBackExternalData", openExternalData_writeBackExternalData)))
    PRINT_ERROR;
*/

	//################################################
  printf("Add suite for tixiGetXXXAttribute\n");

  if (!
      (pSuite =
       CU_add_suite("tixiGetAttribute", preGetAttribute, postGetAttribute)))
    PRINT_ERROR;

  if (!
      (CU_add_test
       (pSuite, "elementNotFound", getTextAttribute_elementNotFound)))
    PRINT_ERROR;
  if (!(CU_add_test(pSuite, "invalidHandle", getTextAttribute_invalidHandle)))
    PRINT_ERROR;
  if (!
      (CU_add_test
       (pSuite, "attributeNotFound", getTextAttribute_attributeNotFound)))
    PRINT_ERROR;
  if (!(CU_add_test(pSuite, "notUnique", getTextAttribute_notUnique)))
    PRINT_ERROR;
  if (!(CU_add_test(pSuite, "getText", getTextAttribute_getText)))
    PRINT_ERROR;
  if (!(CU_add_test(pSuite, "getDouble", getDoubleAttribute)))
    PRINT_ERROR;
  if (!(CU_add_test(pSuite, "getInteger", getIntegerAttribute)))
    PRINT_ERROR;

  /* if ( !( CU_add_test( pSuite, "invalidXPath", getTextElement_invalidXPath )) ) PRINT_ERROR; */


  printf("Add suite for other functions\n");
  if (!
      (pSuite =
       CU_add_suite("tixiOtherFunctions", preOtherFunctions,
                    postOtherFunctions)))
    PRINT_ERROR;

  if (!(CU_add_test(pSuite, "hasNoChild", childCount_hasNoChild)))
    PRINT_ERROR;
  if (!
      (CU_add_test
       (pSuite, "childDoesNotExist", childCount_childDoesNotExist)))
    PRINT_ERROR;
  if (!(CU_add_test(pSuite, "oneChild", childCount_oneChild)))
    PRINT_ERROR;
  if (!(CU_add_test(pSuite, "twoChildren", childCount_twoChildren)))
    PRINT_ERROR;
  if (!
      (CU_add_test
       (pSuite, "addLinkwithAttribute", addExternalLink_withAttribute)))
    PRINT_ERROR;
  if (!
      (CU_add_test
       (pSuite, "addLinkwithoutAttribute", addExternalLink_withoutAttribute)))
    PRINT_ERROR;
  if (!(CU_add_test(pSuite, "getMatrixSize", getMatrixSize)))
    PRINT_ERROR;
  if (!(CU_add_test(pSuite, "createMatrix", createMatrix)))
    PRINT_ERROR;
  if (!(CU_add_test(pSuite, "checkDocumentHandle", checkDocumentHandle)))
    PRINT_ERROR;
  if (!(CU_add_test(pSuite, "checkElement_success", checkElement_success)))
    PRINT_ERROR;
  if (!(CU_add_test(pSuite, "checkElement_failed", checkElement_failed)))
    PRINT_ERROR;

  printf("Add suite for createDocument and addHeader.\n");

  if (!(pSuite = CU_add_suite("tixiCreateDocument", NULL, NULL)))
    PRINT_ERROR;

  if (!(CU_add_test(pSuite, "noElementName", createDocument_noElementName)))
    PRINT_ERROR;
  if (!
      (CU_add_test
       (pSuite, "invalidRootName", createDocument_invalidRootNodeName)))
    PRINT_ERROR;
  if (!(CU_add_test(pSuite, "createDocument", createDocument_success)))
    PRINT_ERROR;
  if (!(CU_add_test(pSuite, "addHeader", createDocument_addHeader)))
    PRINT_ERROR;


  	//################################################
  printf("Add suite for tixiValidateSchema\n");

  if (!
      (pSuite =
       CU_add_suite("tixiValidateSchema", preValidateSchema, postValidateSchema)))
    PRINT_ERROR;
  if (!(CU_add_test(pSuite, "tixiValidateSchema", tixiValidateSchema)))
    PRINT_ERROR;

  	//################################################

  printf("Add suite for tixiVectorTests\n");

  if (!
      (pSuite =
       CU_add_suite("tixiVectorTests", preVectorTests, postVectorTests)))
    PRINT_ERROR;
  if (!(CU_add_test(pSuite, "tixiVectorGetTests", tixiVectorGetTests)))
    PRINT_ERROR;
  if (!(CU_add_test(pSuite, "tixiVectorAddTests", tixiVectorAddTests)))
    PRINT_ERROR;

  	//################################################
  printf("Add suite for tixiArrayTests\n");

  if (!
      (pSuite =
       CU_add_suite("tixiArrayTests", preArrayTests, postArrayTests)))
    PRINT_ERROR;
  if (!(CU_add_test(pSuite, "tixiArrayTestsPositive", tixiArrayTestsPositive)))
    PRINT_ERROR;
  if (!(CU_add_test(pSuite, "tixiArrayTestsNegative", tixiArrayTestsNegative)))
    PRINT_ERROR;

  	//################################################

  printf("Add suite for xPath testing.\n");

  if (!
       (pSuite = CU_add_suite("tixiXPaths", /* pre */ NULL, /* post */ NULL)))
    PRINT_ERROR;

  if (!(CU_add_test(pSuite, "generateXPath", xPathGeneration)))
    PRINT_ERROR;

  if (!(CU_add_test(pSuite, "xPathExpressionEvaluation", xPathExpressionEvaluation)))
    PRINT_ERROR;
  

  	//################################################

  printf("Add suite for XSLT testing.\n");

  if (!
       (pSuite = CU_add_suite("tixiXSLT", /* pre */ NULL, /* post */ NULL)))
    PRINT_ERROR;

  if (!(CU_add_test(pSuite, "xslTransformationToFile", xslTransformationToFile)))
    PRINT_ERROR;


  	//################################################

  printf("Add suite for adding elements.\n");

  if (!
      (pSuite =
       CU_add_suite("tixiAddElement", preAddElement, postAddElement)))
    PRINT_ERROR;

  if (!(CU_add_test(pSuite, "parentRoot", addElement_parentRoot)))
    PRINT_ERROR;
  if (!(CU_add_test(pSuite, "addTree", addElement_addTree)))
    PRINT_ERROR;
  /*  To be implemented: add all parent elements if they do not already exist !! */
  /*    if ( !( CU_add_test( pSuite, "addElementAndParents", addElement_addElementAndParents)) ) PRINT_ERROR; */
  if (!(CU_add_test(pSuite, "invalidName", addElement_invalidName)))
    PRINT_ERROR;
  if (!(CU_add_test(pSuite, "parentNotUnique", addElement_parentNotUnique)))
    PRINT_ERROR;
  if (!(CU_add_test(pSuite, "parentNotFound", addElement_parentNotFound)))
    PRINT_ERROR;
  if (!(CU_add_test(pSuite, "addDouble", addElement_addDouble)))
    PRINT_ERROR;
  if (!(CU_add_test(pSuite, "addInteger", addElement_addInteger)))
    PRINT_ERROR;
  if (!(CU_add_test(pSuite, "addBoolean", addElement_addBoolean)))
    PRINT_ERROR;
  if (!(CU_add_test(pSuite, "addPoint", addElement_addPoint)))
    PRINT_ERROR;
  if (!(CU_add_test(pSuite, "addMatrix", addElement_addMatrix)))
    PRINT_ERROR;
  if (!
      (CU_add_test
       (pSuite, "addMatrixOfPoints", addElement_addMatrixOfPoints)))
    PRINT_ERROR;

  if (!(CU_add_test(pSuite, "alreadySaved", addElement_alreadySaved)))
    PRINT_ERROR;
#if __unix__ || unix
  if (!(CU_add_test(pSuite, "compareResult", addElement_compareResult)))
    PRINT_ERROR;
#endif /* __unix__ */

  printf("Add suite for adding complex elements.\n");

  if (!
      (pSuite =
       CU_add_suite("tixiAddDoubleListWithAttributes", preAddElementList,
                    NULL)))
    PRINT_ERROR;

  if (!(CU_add_test(pSuite, "success", addElementList_success)))
    PRINT_ERROR;


  printf("Add suite for adding attributes.\n");

  if (!(pSuite = CU_add_suite("tixiAddAttribute", preAddAttribute, NULL)))
    PRINT_ERROR;


  if (!(CU_add_test(pSuite, "text", addAttribute_text)))
    PRINT_ERROR;
  if (!(CU_add_test(pSuite, "double", addAttribute_double)))
    PRINT_ERROR;
  if (!(CU_add_test(pSuite, "integer", addAttribute_integer)))
    PRINT_ERROR;

  
  	//################################################

  printf("Add suite for UID testing.\n");

  if (!
       (pSuite = CU_add_suite("tixiUID", /* pre */ NULL, /* post */ NULL)))
    PRINT_ERROR;

  if (!(CU_add_test(pSuite, "tixiUIDCheck_ok", tixiUIDCheck_ok)))
    PRINT_ERROR;

  if (!(CU_add_test(pSuite, "tixiUIDCheck_duplicates", tixiUIDCheck_duplicates)))
    PRINT_ERROR;

  if (!(CU_add_test(pSuite, "tixiUIDCheck_getXPath", tixiUIDCheck_getXPath)))
    PRINT_ERROR;

  if (!(CU_add_test(pSuite, "tixiUIDCheck_getXPathFailed", tixiUIDCheck_getXPathFailed)))
    PRINT_ERROR;

  if (!(CU_add_test(pSuite, "tixiUIDCheck_checkExists", tixiUIDCheck_checkExists)))
    PRINT_ERROR;

  if (!(CU_add_test(pSuite, "tixiUIDCheck_setToXpath", tixiUIDCheck_setToXpath)))
    PRINT_ERROR;

  if (!(CU_add_test(pSuite, "tixiUIDCheck_checkLink", tixiUIDCheck_checkLink)))
    PRINT_ERROR;

  //################################################



#if __unix__ || unix
  if (!(CU_add_test(pSuite, "compare", addAttribute_compareResult)))
    PRINT_ERROR;
#endif /* __unix__ */

#if FORTRAN

  printf("Add suite for testing the fortran interface.\n");

  if (!(pSuite = CU_add_suite("fortranInterface", NULL, NULL)))
    PRINT_ERROR;

  if (!
      (CU_add_test(pSuite, "makeCString_success ", test_makeCString_success)))
    PRINT_ERROR;
  if (!
      (CU_add_test
       (pSuite, "makeCString_emptyString", test_makeCString_emptyString)))
    PRINT_ERROR;

  if (!
      (CU_add_test
       (pSuite, "openDocument", fTest_OpenAndCloseDocument_success)))
    PRINT_ERROR;
  if (!(CU_add_test(pSuite, "getElement_success", fTest_getElement_success)))
    PRINT_ERROR;
  if (!(CU_add_test(pSuite, "addTree_success", fTest_addElement_addTree)))
    PRINT_ERROR;
  if (!(CU_add_test(pSuite, "getPoint", fTest_get_point)))
    PRINT_ERROR;
  if (!(CU_add_test(pSuite, "getNotAPoint", fTest_get_not_a_point)))
    PRINT_ERROR;
  if (!(CU_add_test(pSuite, "addPoint", fTest_add_point)))
    PRINT_ERROR;
  if (!(CU_add_test(pSuite, "addFloatMatrix", fTest_add_matrix)))
    PRINT_ERROR;
  if (!(CU_add_test(pSuite, "checkDocument", fTest_check_document)))
    PRINT_ERROR;
  if (!(CU_add_test(pSuite, "checkElement", fTest_check_element)))
    PRINT_ERROR;

#endif /* FORTRAN  */


}
