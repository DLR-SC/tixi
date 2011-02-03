/* 
* Copyright (C) 2007-2011 German Aerospace Center (DLR/SC)
*
* Created: 2010-08-13 Markus Litz <Markus.Litz@dlr.de>
* Changed: $Id: fortrantests.h 4392 2011-02-03 08:52:55Z litz_ma $ 
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
   @file Fortran name translation
*/

#ifndef FORTRAN_TEST_H
#define FORTRAN_TEST_H

#if defined G77_LINUX || defined G77_IRIX || defined G77_MINGW
#define FORTRAN_NAME(a) a##__
#endif /* defined G77_LINUX || defined G77_IRIX || defined G77_MINGW */

#if defined F90_HPUX
#define FORTRAN_NAME(a) a
#endif /* defined F90_HPUX */

#if defined F90_IRIX
#define FORTRAN_NAME(a) a##_
#endif /* defined F90_IRIX */

#if !defined IFORT_WIN32

#define ftest_open_document_success FORTRAN_NAME(ftest_open_document_success)
#define ftest_close_document_success FORTRAN_NAME(ftest_close_document_success)
#define ftest_get_element_success FORTRAN_NAME(ftest_get_element_success)
#define ftest_get_element_invalid_variable FORTRAN_NAME(ftest_get_element_invalid_variable)
#define ftest_add_element_add_tree FORTRAN_NAME(ftest_add_element_add_tree)
#define ftest_add_point FORTRAN_NAME(ftest_add_point)
#define ftest_get_added_point FORTRAN_NAME(ftest_get_added_point)
#define ftest_get_point FORTRAN_NAME(ftest_get_point)
#define ftest_get_not_a_point FORTRAN_NAME(ftest_get_not_a_point)
#define ftest_add_matrix FORTRAN_NAME(ftest_add_matrix)
#define ftest_get_matrix_size FORTRAN_NAME(ftest_get_matrix_size)
#define ftest_get_float_matrix FORTRAN_NAME(ftest_get_float_matrix)
#define ftest_check_document_success  FORTRAN_NAME(ftest_check_document_success)
#define ftest_check_document_failed FORTRAN_NAME(ftest_check_document_failed)
#define ftest_check_element_success  FORTRAN_NAME(ftest_check_element_success)
#define ftest_check_element_failed FORTRAN_NAME(ftest_check_element_failed)

#else /*  !defined IFORT_WIN32 */

#define ftest_open_document_success FTEST_OPEN_DOCUMENT_SUCCESS
#define ftest_close_document_success FTEST_CLOSE_DOCUMENT_SUCCESS
#define ftest_get_element_success FTEST_GET_ELEMENT_SUCCESS
#define ftest_get_element_invalid_variable FTEST_GET_ELEMENT_INVALID_VARIABLE
#define ftest_add_element_add_tree FTEST_ADD_ELEMENT_ADD_TREE
#define ftest_add_point FTEST_ADD_POINT
#define ftest_get_added_point FTEST_GET_ADDED_POINT
#define ftest_get_point FTEST_GET_POINT
#define ftest_get_not_a_point FTEST_GET_NOT_A_POINT
#define ftest_add_matrix FTEST_ADD_MATRIX
#define ftest_get_matrix_size FTEST_GET_MATRIX_SIZE
#define ftest_get_float_matrix FTEST_GET_FLOAT_MATRIX
#define ftest_check_document_success  FTEST_CHECK_DOCUMENT_SUCCESS
#define ftest_check_document_failed FTEST_CHECK_DOCUMENT_FAILED
#define ftest_check_element_success  FTEST_CHECK_ELEMENT_SUCCESS
#define ftest_check_element_failed FTEST_CHECK_ELEMENT_FAILED

#endif /*  !defined IFORT_WIN32 */

void ftest_open_document_success( int *error );
void ftest_close_document_success( int *error );
void ftest_get_element_success( char* name, int *error, int length );
void ftest_add_element_add_tree( int *error );
void ftest_add_point( int *error );
void ftest_get_point( double* x, double* y, double* z, int *error );
void ftest_get_not_a_point( int* error );
void ftest_get_added_point( double* x, double* y, double* z, int* error );
void ftest_add_matrix( int *error );
void ftest_get_matrix_size( int* nRows, int* nColumns, int* error );
void ftest_get_float_matrix( double* array, int* error );
void ftest_check_document_success( int* error);
void ftest_check_document_failed( int* error);
void ftest_check_element_success( int* error);
void ftest_check_element_failed( int* error);

#endif /* FORTRAN_TEST_H */
