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
/**
  @brief Declaration of the test functions.
*/
#ifndef TESTFUNCTIONS_H
#define TESTFUNCTIONS_H

void openDocument_nonExisting( void ); 
void openDocument_notWellFormed( void );  
void openDocument_isWellFormed( void ); 
void openDocument_checkHandle( void ); 
void openDocument_openHttpURL( void );

void closeDocument_invalidHandle( void );
void closeDocument_close( void );
void closeDocument_closeMultiple( void );

int preGetElement( void );
int postGetElement( void );
void getTextElement_getText( void );
void getEmptyElement( void );
void getTextElement_elementNotFound( void );
void getTextElement_notUnique( void );
void getTextElement_invalidHandle( void );
void getTextElement_invalidXPath( void );
void getTextElement_notAnElement( void );
void getTextElement_useIndexedPath( void );
void getDoubleElement( void );
void getIntegerElement( void );
void getBooleanElement( void );
void getPointElement( void );
void getFloatMatrixElement( void );
void getElement_getMatrixOfPoints( void );

int preRemoveElement( void );
int postRemoveElement( void );
void  getRemoveElement_singleElement( void );

int preRemoveAttribute( void );
int postRemoveAttribute( void );
void  getRemoveAttribute_singleAttribute( void );

int preOpenExternalData( void );
int postOpenExternalData( void );
void openExternalData_getExternalData( void );
void openExternalData_writeBackExternalData( void );

int preGetAttribute( void );
int postGetAttribute( void );
void getTextAttribute_getText( void );
void getTextAttribute_elementNotFound( void );
void getTextAttribute_attributeNotFound( void );
void getTextAttribute_notUnique( void );
void getTextAttribute_invalidHandle( void );
void getDoubleAttribute( void );
void getIntegerAttribute( void );

int preOtherFunctions( void );
int postOtherFunctions( void );
void childCount_hasNoChild(void );
void childCount_childDoesNotExist(void );
void childCount_oneChild(void );
void childCount_twoChildren(void );
void addExternalLink_withoutAttribute( void );
void addExternalLink_withAttribute( void );
void getMatrixSize( void );
void createMatrix( void );
void checkDocumentHandle( void );
void checkElement_success( void );
void checkElement_failed( void );

int preValidateSchema( void );
int postValidateSchema( void );
void tixiValidateSchema( void );

int preVectorTests ( void );
int postVectorTests ( void );
void tixiVectorGetTests ( void );
void tixiVectorAddTests ( void );

int preArrayTests( void );
int postArrayTests( void );
void tixiArrayTestsPositive( void );
void tixiArrayTestsNegative( void );

void xPathGeneration( void );
void xPathExpressionEvaluation( void );

void xslTransformationToFile( void );

void tixiUIDCheck_ok( void );
void tixiUIDCheck_duplicates( void );
void tixiUIDCheck_getXPath( void );
void tixiUIDCheck_getXPathFailed( void );
void tixiUIDCheck_checkExists( void );
void tixiUIDCheck_setToXpath( void );
void tixiUIDCheck_checkLink( void );

int preCreateDocument( void );
void createDocument_noElementName ( void );
void createDocument_invalidRootNodeName( void );
void createDocument_success( void );
void createDocument_addHeader( void );


int preAddElement( void );
int postAddElement( void );
void addElement_parentRoot( void );
void addElement_addElementAndParents( void );
void addElement_addTree( void );
void addElement_invalidName( void );
void addElement_parentNotUnique( void );
void addElement_parentNotFound( void );
void addElement_alreadySaved( void );
void addElement_addDouble( void );
void addElement_addInteger( void );
void addElement_addBoolean( void );
void addElement_addPoint( void );
void addElement_addMatrix( void );
void addElement_addMatrixOfPoints( void );

#if __unix__ || unix || __APPLE__
void addElement_compareResult( void );
#endif /* __unix__ */

int preAddElementList( void );
void addElementList_success( void );

int preAddAttribute( void );
void addAttribute_text( void );
void addAttribute_double( void );
void addAttribute_integer( void );

#if __unix__ || unix || __APPLE__
void addAttribute_compareResult( void );
#endif /* __unix__ */

#if FORTRAN
void fTest_OpenAndCloseDocument_success( void );
void test_makeCString_success( void );
void test_makeCString_emptyString( void );
void fTest_getElement_success(void);
void fTest_addElement_addTree(void);
void fTest_get_point(void);
void fTest_get_not_a_point(void);
void fTest_add_point(void);
void fTest_add_matrix( void );
void  fTest_check_document(void) ;
void  fTest_check_element(void) ;
#endif /* FORTRAN */

void buildString_success(void);

#endif
