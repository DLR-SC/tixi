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
* � � http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
/** @file This is the header file for the fortran wrapper for the tixi functions from the fortran interface
*/

/*
    g77 on Linux, MinGW, IRIX:
    Append one underscore if the name does not contain an underscore otherwise
    append two. All names are in lower case.

    HP-UX:
    All names are in lower case.

    f90/f77 on IRIX:
    Append one underscore

    ifort/cvf
    All names are upper-case
*/
#ifndef TIXI77_H
#define TIXI77_H

#include "tixi.h"

#if defined G77_LINUX || defined G77_IRIX || defined G77_MINGW || defined G77
	#define FORTRAN_NAME(a) a##__
#elif defined F90_HPUX
	#define FORTRAN_NAME(a) a
#elif defined F90_IRIX || defined GFORTRAN
	#define FORTRAN_NAME(a) a##_
#else
	#define FORTRAN_NAME(a) a
#endif


#ifdef IFORT_WIN32

#define tixiGetVersion_f TIXI_GET_VERSION
#define tixiOpenDocument_f TIXI_OPEN_DOCUMENT
#define tixiOpenDocumentRecursive_f TIXI_OPEN_DOCUMENT_RECURSIVE
#define tixiOpenDocumentFromHTTP_f TIXI_OPEN_DOCUMENT_FROM_HTTP
#define tixiCreateDocument_f TIXI_CREATE_DOCUMENT
#define tixiSaveDocument_f TIXI_SAVE_DOCUMENT
#define tixiSaveCompleteDocument_f TIXI_SAVE_COMPLETE_DOCUMENT
#define tixiSaveAndRemoveDocument_f TIXI_SAVE_AND_REMOVE_DOCUMENT
#define tixiCloseDocument_f TIXI_CLOSE_DOCUMENT
#define tixiExportDocumentAsString_f TIXI_EXPORT_DOCUMENT_AS_STRING
#define tixiImportFromString_f TIXI_IMPORT_FROM_STRING
#define tixiSchemaValidateFromFile_f TIXI_SCHEMA_VALIDATE_FROM_FILE
#define tixiSchemaValidateFromString_f TIXI_SCHEMA_VALIDATE_FROM_STRING
#define tixiDTDValidate_f TIXI_DTD_VALIDATE
#define tixiGetTextElement_f TIXI_GET_TEXT_ELEMENT
#define tixiGetBooleanElement_f TIXI_GET_BOOLEAN_ELEMENT
#define tixiGetIntegerElement_f TIXI_GET_INTEGER_ELEMENT
#define tixiGetDoubleElement_f TIXI_GET_DOUBLE_ELEMENT
#define tixiAddTextElement_f TIXI_ADD_TEXT_ELEMENT
#define tixiAddTextElementAtIndex_f TIXI_ADD_TEXT_ELEMENT_AT_INDEX
#define tixiAddDoubleElement_f TIXI_ADD_DOUBLE_ELEMENT
#define tixiAddIntegerElement_f TIXI_ADD_INTEGER_ELEMENT
#define tixiAddBooleanElement_f TIXI_ADD_BOOLEAN_ELEMENT
#define tixiUpdateTextElement_f TIXI_UPDATE_TEXT_ELEMENT
#define tixiUpdateDoubleElement_f TIXI_UPDATE_DOUBLE_ELEMENT
#define tixiUpdateIntegerElement_f TIXI_UPDATE_INTEGER_ELEMENT
#define tixiUpdateBooleanElement_f TIXI_UPDATE_BOOLEAN_ELEMENT
#define tixiRemoveElement_f TIXI_REMOVE_ELEMENT
#define tixiGetTextAttribute_f TIXI_GET_TEXT_ATTRIBUTE
#define tixiGetIntegerAttribute_f TIXI_GET_INTEGER_ATTRIBUTE
#define tixiGetBooleanAttribute_f TIXI_GET_BOOLEAN_ATTRIBUTE
#define tixiGetDoubleAttribute_f TIXI_GET_DOUBEL_ATTRIBUTE
#define tixiAddTextAttribute_f TIXI_ADD_TEXT_ATTRIBUTE
#define tixiAddDoubleAttribute_f TIXI_ADD_DOUBLE_ATTRIBUTE
#define tixiAddIntegerAttribute_f TIXI_ADD_INTEGER_ATTRIBUTE
#define tixiRemoveAttribute_f TIXI_REMOVE_ATTRIBUTE
#define tixiAddHeader_f TIXI_ADD_HEADER
#define tixiGetNamedChildrenCount_f TIXI_GET_NAMED_CHILDREN_COUNT
#define tixiAddDoubleListWithAttributes_f TIXI_ADD_DOUBLE_LIST_WITH_ATTRIBUTES
#define tixiAddExternalLink_f TIXI_ADD_EXTERNAL_LINK
#define tixiAddPoint_f TIXI_ADD_POINT
#define tixiGetPoint_f TIXI_GET_POINT
#define tixiCreateMatrix_f TIXI_CREATE_MATRIX
#define tixiAddFloatMatrix_f TIXI_ADD_FLOAT_MATRIX
#define tixiGetFloatMatrix_f TIXI_GET_FLOAT_MATRIX
#define tixiGetMatrixSize_f TIXI_GET_MATRIX_SIZE
#define tixiCheckElement_f TIXI_CHECK_ELEMENT
#define tixiCheckDocumentHandle_f TIXI_CHECK_DOCUMENT_HANDLE
#define tixiAddFloatVector_f TIXI_ADD_FLOAT_VECTOR
#define tixiGetVectorSize_f TIXI_GET_VECTOR_SIZE
#define tixiGetFloatVector_f TIXI_GET_FLOAT_VECTOR
#define tixiGetArrayDimensions_f TIXI_GET_ARRAY_DIMENSIONS
#define tixiGetArrayDimensionNames_f TIXI_GET_ARRAY_DIMENSION_NAMES
#define tixiGetArrayDimensionSizes_f TIXI_GET_ARRAY_DIMENSION_SIZES
#define tixiGetArrayDimensionValues_f TIXI_GET_ARRAY_DIMENSION_VALUES
#define tixiGetArrayParameters_f TIXI_GET_ARRAY_PARAMETERS
#define tixiGetArrayParameterNames_f TIXI_GET_ARRAY_PARAMETER_NAMES
#define tixiGetArray_f TIXI_GET_ARRAY
#define tixiGetArrayValue_f TIXI_GET_ARRAY_VALUE
#define tixiCreateElement_f TIXI_CREATE_ELEMENT
#define tixiCreateElementAtIndex_f TIXI_CREATE_ELEMENT_AT_INDEX
#define tixiCheckAttribute_f TIXI_CHECK_ATTRIBUTE
#define tixiXSLTransformationToFile_f TIXI_XSL_TRANSFORMATION_TO_FILE
#define tixiUsePrettyPrint_f TIXI_USE_PRETTY_PRINT
#define tixiUIDCheckDuplicates_f TIXI_UID_CHECK_DUPLICATED
#define tixiUIDCheckLinks_f TIXI_UID_CHECK_LINKS
#define tixiUIDGetXPath_f TIXI_UID_GET_XPATH
#define tixiUIDCheckExists_f TIXI_UID_CHECK_EXISTS
#define tixiUIDSetToXPath_f TIXI_UID_SET_TO_XPATH
#define tixiXPathEvaluateNodeNumber_f TIXI_XPATH_EVALUATE_NODE_NUMBER
#define tixiXPathExpressionGetTextByIndex_f TIXI_XPATH_EXPRESSION_GET_TEXT_BY_INDEX
#define tixiGetChildElementName_f TIXI_GET_CHILD_ELEMENT_NAME
#define tixiGetNumberOfChilds_f TIXI_GET_NUMBER_OF_CHILDS
#define tixiGetNumberOfAttributes_f TIXI_GET_NUMBER_OF_ATTRIBUTES
#define tixiGetAttributeName_f TIXI_GET_ATTRIBUTE_NAME

#else /* (not) IFORT_WIN32 */

#define tixiGetVersion_f FORTRAN_NAME(tixi_get_version)
#define tixiOpenDocument_f FORTRAN_NAME(tixi_open_document)
#define tixiOpenDocumentRecursive_f FORTRAN_NAME(tixi_open_document_recursive)
#define tixiOpenDocumentFromHTTP_f FORTRAN_NAME(tixi_open_document_from_http)
#define tixiCreateDocument_f FORTRAN_NAME(tixi_create_document)
#define tixiSaveDocument_f FORTRAN_NAME(tixi_save_document)
#define tixiSaveCompleteDocument_f FORTRAN_NAME(tixi_save_complete_document)
#define tixiSaveAndRemoveDocument_F FORTRAN_NAME(tixi_save_and_remove_document)
#define tixiCloseDocument_f FORTRAN_NAME(tixi_close_document)
#define tixiExportDocumentAsString_f FORTRAN_NAME(tixi_export_document_as_string)
#define tixiImportFromString_f FORTRAN_NAME(tixi_import_from_string)
#define tixiSchemaValidateFromFile_f FORTRAN_NAME(tixi_schema_validate_from_file)
#define tixiSchemaValidateFromString_f FORTRAN_NAME(tixi_schema_validate_from_string)
#define tixiDTDValidate_f FORTRAN_NAME(tixi_dtd_validata)
#define tixiGetTextElement_f FORTRAN_NAME(tixi_get_text_element)
#define tixiGetBooleanElement_f FORTRAN_NAME(tixi_get_boolean_element)
#define tixiGetIntegerElement_f FORTRAN_NAME(tixi_get_integer_element)
#define tixiGetDoubleElement_f FORTRAN_NAME(tixi_get_double_element)
#define tixiAddTextElement_f FORTRAN_NAME(tixi_add_text_element)
#define tixiAddTextElementAtIndex_f FORTRAN_NAME(tixi_add_text_element_at_index)
#define tixiAddDoubleElement_f FORTRAN_NAME(tixi_add_double_element)
#define tixiAddIntegerElement_f FORTRAN_NAME(tixi_add_integer_element)
#define tixiAddBooleanElement_f FORTRAN_NAME(tixi_add_boolean_element)
#define tixiUpdateTextElement_f FORTRAN_NAME(tixi_update_text_element)
#define tixiUpdateDoubleElement_f FORTRAN_NAME(tixi_update_double_element)
#define tixiUpdateIntegerElement_f FORTRAN_NAME(tixi_update_integer_element)
#define tixiUpdateBooleanElement_f FORTRAN_NAME(tixi_update_boolean_element)
#define tixiRemoveElement_f FORTRAN_NAME(tixi_remove_element)
#define tixiGetTextAttribute_f FORTRAN_NAME(tixi_get_text_attribute)
#define tixiGetIntegerAttribute_f FORTRAN_NAME(tixi_get_integer_attribute)
#define tixiGetBooleanAttribute_f FORTRAN_NAME(tixi_get_boolean_attribute)
#define tixiGetDoubleAttribute_f FORTRAN_NAME(tixi_get_doubel_attribute)
#define tixiAddTextAttribute_f FORTRAN_NAME(tixi_add_text_attribute)
#define tixiAddDoubleAttribute_f FORTRAN_NAME(tixi_add_double_attribute)
#define tixiAddIntegerAttribute_f FORTRAN_NAME(tixi_add_integer_attribute)
#define tixiRemoveAttribute_f FORTRAN_NAME(tixi_remove_attribute)
#define tixiAddHeader_f FORTRAN_NAME(tixi_add_header)
#define tixiGetNamedChildrenCount_f FORTRAN_NAME(tixi_get_named_children_count)
#define tixiAddDoubleListWithAttributes_f FORTRAN_NAME(tixi_add_double_list_with_attributes)
#define tixiAddExternalLink_f FORTRAN_NAME(tixi_add_external_link)
#define tixiAddPoint_f FORTRAN_NAME(tixi_add_point)
#define tixiGetPoint_f FORTRAN_NAME(tixi_get_point)
#define tixiCreateMatrix_f FORTRAN_NAME(tixi_create_matrix)
#define tixiAddFloatMatrix_f FORTRAN_NAME(tixi_add_float_matrix)
#define tixiGetFloatMatrix_f FORTRAN_NAME(tixi_get_float_matrix)
#define tixiGetMatrixSize_f FORTRAN_NAME(tixi_get_matrix_size)
#define tixiCheckElement_f FORTRAN_NAME(tixi_check_element)
#define tixiCheckDocumentHandle_f FORTRAN_NAME(tixi_check_document_handle)
#define tixiAddFloatVector_f FORTRAN_NAME(tixi_add_float_vector)
#define tixiGetVectorSize_f FORTRAN_NAME(tixi_get_vector_size)
#define tixiGetFloatVector_f FORTRAN_NAME(tixi_get_float_vector)
#define tixiGetArrayDimensions_f FORTRAN_NAME(tixi_get_array_dimensions)
#define tixiGetArrayDimensionNames_f FORTRAN_NAME(tixi_get_array_dimension_names)
#define tixiGetArrayDimensionSizes_f FORTRAN_NAME(tixi_get_array_dimension_sizes)
#define tixiGetArrayDimensionValues_f FORTRAN_NAME(tixi_get_array_dimension_values)
#define tixiGetArrayParameters_f FORTRAN_NAME(tixi_get_array_parameters)
#define tixiGetArrayParameterNames_f FORTRAN_NAME(tixi_get_array_parameter_names)
#define tixiGetArray_f FORTRAN_NAME(tixi_get_array)
#define tixiGetArrayValue_f FORTRAN_NAME(tixi_get_array_value)
#define tixiCreateElement_f FORTRAN_NAME(tixi_create_element)
#define tixiCreateElementAtIndex_f FORTRAN_NAME(tixi_create_element_at_index)
#define tixiCheckAttribute_f FORTRAN_NAME(tixi_check_attribute)
#define tixiXSLTransformationToFile_f FORTRAN_NAME(tixi_xsl_transformation_to_file)
#define tixiUsePrettyPrint_f FORTRAN_NAME(tixi_use_pretty_print)
#define tixiUIDCheckDuplicates_f FORTRAN_NAME(tixi_uid_check_duplicated)
#define tixiUIDCheckLinks_f FORTRAN_NAME(tixi_uid_check_links)
#define tixiUIDGetXPath_f FORTRAN_NAME(tixi_uid_get_xpath)
#define tixiUIDCheckExists_f FORTRAN_NAME(tixi_uid_check_exists)
#define tixiUIDSetToXPath_f FORTRAN_NAME(tixi_uid_set_to_xpath)
#define tixiXPathEvaluateNodeNumber_f FORTRAN_NAME(tixi_xpath_evaluate_node_number)
#define tixiXPathExpressionGetTextByIndex_f FORTRAN_NAME(tixi_xpath_expression_get_text_by_index)
#define tixiGetChildElementName_f FORTRAN_NAME(tixi_get_child_element_name)
#define tixiGetNumberOfChilds_f FORTRAN_NAME(tixi_get_number_of_childs)
#define tixiGetNumberOfAttributes_f FORTRAN_NAME(tixi_get_number_of_attributes)
#define tixiGetAttributeName_f FORTRAN_NAME(tixi_get_attribute_name)

#endif /* IFORT_WIN32 */

void tixiGetVersion_f(char* version,
                      int stringLength );

void tixiOpenDocument_f(const char* xmlFilename,
                        int* handle,
                        ReturnCode* error,
                        int lengthString1 );

void tixiOpenDocumentRecursive_f(const char* xmlFilename,
                                int* handle,
                                ReturnCode* error,
                                OpenMode *oMode,
                                const int lengthString1 );

void tixiOpenDocumentFromHTTP_f(const char* xmlUrl,
                                int* handle,
                                ReturnCode* error,
                                int lengthString1 );


void tixiCreateDocument_f(char* rootElementName,
                          TixiDocumentHandle *handle,
                          ReturnCode* error,
                          int lengthString1 );

void tixiSaveDocument_f(TixiDocumentHandle* handle,
                        char* xmlFilename,
                        ReturnCode* error,
                        int lengthString1 );

void tixiSaveCompleteDocument_f(TixiDocumentHandle* handle,
                                char* xmlFilename,
                                ReturnCode* error,
                                int lengthString1 );

void tixiSaveAndRemoveDocument_f(TixiDocumentHandle* handle,
                                char* xmlFilename,
                                ReturnCode* error,
                                int lengthString1 );

void tixiCloseDocument_f(TixiDocumentHandle* handle,
                         ReturnCode* error );

void tixiExportDocumentAsString_f(TixiDocumentHandle* handle,
                                  char* text,
                                  ReturnCode* error,
                                  int lengthString1);

void tixiImportFromString_f(const char* xmlImportString,
                            int* handle,
                            ReturnCode* error,
                            int lengthString1 );

void tixiSchemaValidateFromFile_f(const TixiDocumentHandle* handle,
                          char* xsdFilename,
                          ReturnCode* error,
                          int lengthString1 );

void tixiSchemaValidateFromString_f(const TixiDocumentHandle* handle,
                          char* xsdString,
                          ReturnCode* error,
                          int lengthString1 );

void tixiDTDValidate_f(const TixiDocumentHandle* handle,
                       char* DTDFilename,
                       ReturnCode* error,
                       const int lengthString1 );

void tixiGetTextElement_f(const TixiDocumentHandle* handle,
                          char* elementPath,
                          char* text,
                          ReturnCode* error,
                          int lengthString1,
                          int lengthString2 );

void tixiGetBooleanElement_f(const TixiDocumentHandle* handle,
                            char* elementPath,
                            int* boolean,
                            ReturnCode* error,
                            int lengthString1);

void tixiGetIntegerElement_f(const TixiDocumentHandle* handle,
                             char* elementPath,
                             int* number,
                             ReturnCode* error,
                             int elementPathLength );

void tixiGetDoubleElement_f(const TixiDocumentHandle* handle,
                            char* elementPath,
                            double* number,
                            ReturnCode* error,
                            int elementPathLength );

void tixiAddTextElement_f(const TixiDocumentHandle* handle,
                          char* parentPath,
                          char* elementName,
                          char* text,
                          ReturnCode* error,
                          int lengthString1,
                          int lengthString2,
                          int lengthString3 );

void tixiAddTextElementAtIndex_f(const TixiDocumentHandle* handle,
								  char* parentPath,
								  char* elementName,
								  char* text,
								  int index,
								  ReturnCode* error,
								  int lengthString1,
								  int lengthString2,
								  int lengthString3 );

void tixiAddDoubleElement_f(const TixiDocumentHandle* handle,
                            char* parentPath,
                            char* elementName,
                            double* number,
                            char* format,
                            ReturnCode* error,
                            int parentPathLength,
                            int elementNameLength,
                            int formatLength );

void tixiAddIntegerElement_f(const TixiDocumentHandle* handle,
                           char* parentPath,
                           char* elementName,
                           int* number,
                            char* format,
                            ReturnCode* error,
                            int parentPathLength,
                            int elementNameLength,
                            int formatLength );

void tixiAddBooleanElement_f(const TixiDocumentHandle* handle,
                             char* parentPath,
                             char* elementName,
                             int* boolean,
                             char* format,
                             ReturnCode* error,
                             int parentPathLength,
                             int elementNameLength,
                             int formatLength );

void tixiUpdateTextElement_f(const TixiDocumentHandle* handle,
							  char* parentPath,
							  char* text,
							  ReturnCode* error,
							  int lengthString1,
							  int lengthString2);

void tixiUpdateDoubleElement_f(const TixiDocumentHandle* handle,
							  char* parentPath,
							  double number,
							  char* format,
							  ReturnCode* error,
							  int lengthString1,
							  int lengthString2,
							  int lengthString3 );


void tixiUpdateIntegerElement_f(const TixiDocumentHandle* handle,
                              char* parentPath,
                              int number,
                              char* format,
                              ReturnCode* error,
                              int lengthString1,
                              int lengthString2,
                              int lengthString3 );

void tixiUpdateBooleanElement_f(const TixiDocumentHandle* handle,
                              char* parentPath,
                              int boolean,
                              ReturnCode* error,
                              int lengthString1 );

void tixiRemoveElement_f(const TixiDocumentHandle* handle,
                         char* elementPath,
                         ReturnCode* error,
                         int lengthString1 );

void tixiGetTextAttribute_f(const TixiDocumentHandle* handle,
                            char* elementPath,
                            char* attributeName,
                            char* text,
                            ReturnCode* error,
                            int lengthString1,
                            int lengthString2,
                            int lengthString3 );


void tixiGetDoubleAttribute_f(const TixiDocumentHandle* handle,
                              char* elementPath,
                              char* attributeName,
                              double* number,
                              ReturnCode* error,
                              int lengthString1,
                              int lengthString2 );


void tixiGetIntegerAttribute_f(const TixiDocumentHandle* handle,
                               char* elementPath,
                               char* attributeName,
                               int* number,
                               ReturnCode* error,
                               int lengthString1,
                               int lengthString2 );

void tixiGetBooleanAttribute_f(const TixiDocumentHandle* handle,
                               char* elementPath,
                               char* attributeName,
                               int* boolean,
                               ReturnCode* error,
                               int lengthString1,
                               int lengthString2 );

void tixiAddIntegerAttribute_f(const TixiDocumentHandle* handle,
                               char* elementPath,
                               char* attributeName,
                               int* number,
                               char* format,
                               ReturnCode* error,
                               int elementPathLength,
                               int attributeNameLength,
                              int formatLength  );

void tixiAddDoubleAttribute_f(const TixiDocumentHandle* handle,
                              char* elementPath,
                              char* attributeName,
                              double* number,
                              char* format,
                              ReturnCode* error,
                              int elementPathLength,
                              int attributeNameLength,
                              int formatLength );

void tixiAddTextAttribute_f(const TixiDocumentHandle* handle,
                            char* elementPath,
                            char* attributeName,
                            char* text,
                            ReturnCode* error,
                            int lengthString1,
                            int lengthString2,
                            int lengthString3 );

void tixiRemoveAttribute_f(const TixiDocumentHandle* handle,
                           char* elementPath,
                           char* attributeName,
                           ReturnCode* error,
                           int lengthString1,
                           int lengthString2 );


void tixiAddHeader_f(const TixiDocumentHandle* handle,
                     char* toolName,
                     char* version,
                     char* authorName,
                     ReturnCode* error,
                     int lengthString1,
                     int lengthString2,
                     int lengthString3 );


void tixiGetNamedChildrenCount_f(const TixiDocumentHandle* handle,
                                 char* elementPath,
                                 char* childName,
                                 int* count,
                                 ReturnCode* error,
                                 int lengthString1,
                                 int lengthString2 );


void tixiAddExternalLink_f(const TixiDocumentHandle* handle,
                           char* parentPath,
                           char* url,
                           char* fileFormat,
                           ReturnCode* error,
                           int lengthString1,
                           int lengthString2,
                           int lengthString3 );

void tixiCheckDocumentHandle_f(const TixiDocumentHandle* handle,
                               ReturnCode* error );


void tixiCheckElement_f(const TixiDocumentHandle* handle,
                        char* elementPath,
                        ReturnCode* error,
                        int elementPathLength );


void tixiAddDoubleListWithAttributes_f(const TixiDocumentHandle* handle,
                                       char* parentPath,
                                       char* listName,
                                       char* childName,
                                       char* childAttributeName,
                                       double* values,
                                       char* format,
                                       char* attributes,
                                       int* nValues,
                                       ReturnCode* error,
                                       int parentPathLength,
                                       int listNameLength,
                                       int childNameLength,
                                       int childAttributeNameLength,
                                       int formatLength,
                                       int attributesLength );


void tixiAddPoint_f(const TixiDocumentHandle* handle,
                    char* parentPath,
                    double* x,
                    double* y,
                    double* z,
                    char* format,
                    int* error,
                    int parentPathLength,
                    int formatLength );

void tixiGetPoint_f(const TixiDocumentHandle* handle,
                    char* parentPath,
                    int* index,
                    double* x,
                    double* y,
                    double* z,
                    int* error,
                    int parentPathLength );


void tixiCreateMatrix_f(const  TixiDocumentHandle* handle,
                        char* parentPath,
                        char* matrixName,
                        char* rowName,
                        char* columnName,
                        int* nRows,
                        int* nColumns,
                        int* error,
                        int parentPathLength,
                        int matrixNameLength,
                        int rowNameLength,
                        int columnNameLength );


void tixiAddFloatMatrix_f(const TixiDocumentHandle* handle,
                          char* parentPath,
                          char* matrixName,
                          char* rowName,
                          char* columnName,
                          int* nRows,
                          int* nColumns,
                          int* mode,
                          double* array,
                          char* format ,
                          int* error,
                          int parentPathLength,
                          int matrixNameLength,
                          int rowNameLength,
                          int columnNameLength,
                          int formatLength );


void tixiGetFloatMatrix_f(const TixiDocumentHandle* handle,
               char* matrixPath,
               char* rowName,
               char* columnName,
               int* nRows,
               int* nColumns,
               int* mode,
               double* array ,
               int* error,
               int matrixPathLength,
               int rowNameLength,
               int columnNameLength );


void tixiGetMatrixSize_f(const TixiDocumentHandle* handle,
              char* matrixPath,
              char* rowName,
              char* columnName,
              int* nRows,
              int* nColumns ,
              int* error,
              int matrixPathLength,
              int rowNameLength,
              int columnNameLength );

void tixiAddFloatVector_f(const TixiDocumentHandle *handle,
            char *parentPath,
            char *elementName,
            double *vector,
            int *numElements,
            int *error,
            int parentPathLength,
            int elementNameLength);

void tixiGetVectorSize_f(const TixiDocumentHandle *handle,
                        const char *vectorPath,
                        int *nElements,
                        int *error,
                        const int vectorPathLength);

void tixiGetFloatVector_f (const TixiDocumentHandle *handle,
            const char *vectorPath,
            double *vectorArray,
            const int *eNumber,
            int *error,
            const int vectorPathLength);

void tixiGetArrayDimensions_f(const TixiDocumentHandle *handle,
                              const char *arrayPath,
                              int *dimensions,  /* out */
                              int *error,   /* out */
                              const int arrayPathLength);

void tixiGetArrayDimensionNames_f(const TixiDocumentHandle *handle,
                                  const char *arrayPath,
                                  char *dimensionNames,    /* out */
                                  const int *strings,
                                  const int *stringSize,
                                  int *error,   /* out */
                                  const int arrayPathLength);

void tixiGetArrayDimensionSizes_f(const TixiDocumentHandle *handle,
                                  const char *arrayPath,
                                  int *sizes,   /* out */
                                  int *arraySize,   /* out */
                                  int *error,   /* out */
                                  const int arrayPathLength);

void tixiGetArrayDimensionValues_f(const TixiDocumentHandle *handle,
                                   const char *arrayPath,
                                   const int *dimension,
                                   double *dimensionValues, /* out */
                                   int *error,  /* out */
                                   const int arrayPathLength);

void tixiGetArrayParameters_f(const TixiDocumentHandle *handle,
                              const char *arrayPath,
                              int *parameters,  /* out */
                              int *error,   /* out */
                              const int arrayPathLength);

void tixiGetArrayParameterNames_f(const TixiDocumentHandle *handle,
                                  const char *arrayPath,
                                  char *parameterNames,    /* out */
                                  const int *strings,
                                  const int *stringSize,
                                  int *error,   /* out */
                                  const int arrayPathLength);

void tixiGetArray_f(const TixiDocumentHandle *handle,
                    const char *arrayPath,
                    const char *elementName,
                    const int numElements,
                    double *values,
                    int *error, /* out */
                    const int arrayPathLength,
                    const int elementNameLength);

void tixiGetArrayValue_f(const double *array,
                         const int *dimSize,
                         const int *dimPos,
                         const int *dims,
                         double *value);

void tixiCreateElement_f (const TixiDocumentHandle *handle,
                          char *parentPath,
                          char *elementName,
                          int *error,
                          int parentPathLength,
                          int elementNameLength);

void tixiCreateElementAtIndex_f (const TixiDocumentHandle *handle,
								  char *parentPath,
								  char *elementName,
								  int index,
								  int *error,
								  int parentPathLength,
								  int elementNameLength);

void tixiCheckAttribute_f(const TixiDocumentHandle *handle,
						  char *elementPath,
						  char *attributeName,
						  ReturnCode* error,
						  int elementPathLength,
						  int attributeNameLength);


void tixiXSLTransformationToFile_f(TixiDocumentHandle* handle,
                                    char* stylesheetFilename,
                                    char* outputFilename,
                                    ReturnCode* error,
                                    int elementPathLength1,
                                    int elementPathLength2);

void tixiUsePrettyPrint_f(TixiDocumentHandle *handle,
                            int *usePrettyPrint,
                            int *error);


void tixiUIDCheckDuplicates_f(TixiDocumentHandle *handle,
							  int *error);

void tixiUIDCheckLinks_f(TixiDocumentHandle *handle,
							  int *error);

void tixiUIDGetXPath_f(TixiDocumentHandle *handle,
					   char* uID,
					   char* xPath,
					   ReturnCode* error,
					   int lengthString1,
					   int lengthString2);

void tixiUIDCheckExists_f(TixiDocumentHandle *handle,
		   char* uID,
		   ReturnCode* error,
		   int lengthString1);

void tixiUIDSetToXPath_f(const TixiDocumentHandle* handle,
                            char* elementPath,
                            char* uID,
                            ReturnCode* error,
                            int lengthString1,
                            int lengthString2);

void tixiXPathEvaluateNodeNumber_f(const TixiDocumentHandle* handle,
									char* xpathExpression,
									int* number,
									ReturnCode* error,
									int lengthString1);

void tixiXPathExpressionGetTextByIndex_F(const TixiDocumentHandle* handle,
										char* xpathExpression,
										int* index,
										char* text,
										ReturnCode* error,
										int lengthString1,
										int lengthString2);

void tixiGetChildElementName_F(const TixiDocumentHandle* handle,
                                        char* elementPath,
                                        int* index,
                                        char* text,
                                        ReturnCode* error,
                                        int lengthString1,
                                        int lengthString2);

void tixiGetNumberOfChilds_f(const TixiDocumentHandle* handle,
                                        char* elementPath,
                                        int * nChilds,
                                        ReturnCode* error,
                                        int lengthString1);

void tixiGetNumberOfAttributes_f(const TixiDocumentHandle* handle,
                                        char* elementPath,
                                        int * nAttributes,
                                        ReturnCode* error,
                                        int lengthString1);


void tixiGetAttributeName_f(const TixiDocumentHandle* handle,
                            char* elementPath,
                            int* index,
                            char* text,
                            ReturnCode* error,
                            int lengthString1,
                            int lengthString2);
#endif /* TIXI77_H */
