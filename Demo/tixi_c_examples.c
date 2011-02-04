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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "tixi.h"

#define CHECK_ERROR( a, b) checkError((a),(b), __LINE__, __FILE__)

/**
   A simple error handling routine. 
*/

void checkError( ReturnCode error, char* msg, int lineNumber, char* fileName ) {
  if ( error ) {
    fprintf( stderr, "------------------------------\n");
    fprintf( stderr, "%s \n Error %d at line %d in file %s \n", msg, error, lineNumber, fileName);
    fprintf( stderr, "------------------------------\n");
  }
}

/**
   Demonstrates how to read an XML file using TIXI.
 */
void readXML( char* inputFileName ) {

  TixiDocumentHandle handle;
  ReturnCode error;
 
  /* open file for reading */

  error = tixiOpenDocument( inputFileName, &handle );
  CHECK_ERROR( error, "tixiOpenDocument" );

  /* Any error occuring here is fatal! */
  if ( error != SUCCESS ) {
    abort();
  }
   

  /* read some simple elements */

  {
    /* text element */
   
    char *planeName;
    
    error = tixiGetTextElement( handle, "/plane/name", &planeName );
    CHECK_ERROR( error, "tixiGetTextElement" );

    printf( "Plane name: %s\n", planeName );
  }

  {
    /* double element */

    double x;
    
    error = tixiGetDoubleElement( handle, "/plane/coordinateOrigin/x", &x );
    CHECK_ERROR( error, "tixiGetDoubleElement" );

    printf( "Coordinate origin x-coordinate:  %g\n", x );
  }

  {
    /* get the number of wing subelements  of element wings */
    char* parentName = "/plane/wings";
    char* subElementName = "wing";
    int count = -1;
   
    error = tixiGetNamedChildrenCount( handle, parentName, subElementName, &count );
    CHECK_ERROR( error, "tixiGetNamedChildrenCount" );

    printf( "Number of wing elements in wings is %d.\n", count );
  
  }
    
  {
    /* element /plane/wings has 3 wing elements. 
       get the y element of each centerOfGravity element. */
    
    int nWings = 3;
    char* baseName = "/plane/wings/wing";
    char* subElementName = "/centerOfGravity/y";
    int elementNameSize = strlen(subElementName) + strlen(baseName) + 1;
    int iWing;

    /* XPath indizes start at 1 !! */
    for ( iWing = 1; iWing <= nWings; iWing++ ) {
      
      char* elementName = NULL;
      int indexStringSize =  
	ceil( log( (double) ( nWings + 1 ) ) ) /* number of digits of the index */
	+ 2                                      /* opening and closing brackets  */
	+ 1;                                     /* trailing "\0"                 */
 
      char* indexString = (char*) malloc( sizeof(char) * indexStringSize );
      double y;

      sprintf( indexString, "[%1d]", iWing ); 
      
      elementNameSize += strlen(indexString);

      elementName = (char*) malloc( sizeof(char) * elementNameSize ); 
      
      strcpy( elementName, baseName );
      strcat( elementName, indexString );
      strcat( elementName, subElementName );

      error = tixiGetDoubleElement( handle, elementName, &y );
      CHECK_ERROR( error, "tixiGetDoubleElement" );

      printf("Center of gravity y coordinate wing #%2d:%8.3f\n", iWing, y );

      free( indexString );
      free( elementName );

    }
  }

  {
    /* read an attribute */

    char *position = NULL;
    
    /* first try to read an non-existing attribute */
    
    error = tixiGetTextAttribute( handle, "/plane/wings/wing[3]", "pstn", &position );
    CHECK_ERROR( error, "tixiGetTextAttribute" );

    /* Now read an existing attribute. */ 

    error = tixiGetTextAttribute( handle, "/plane/wings/wing[3]", "position", &position );
    CHECK_ERROR( error, "tixiGetTextAttribute" );

    printf("Position of wing #3 is \"%s\".\n", position );
 
  }
  
  

  {
    /* read a 2x3 matrix */

    int nRows = -1;
    int nCols = -1;
    double* array = NULL;
    int i;
    int j; 

    printf("\nReading Matrix\n");

    /* 
       To allocate the correct amount of memory for the matrix first
       get the matrix size.
    */
    error = tixiGetMatrixSize( handle, "/plane/two_by_three_matrix", "blub", "bla", 
			       &nRows, &nCols );
    CHECK_ERROR( error, "tixiGetMatrixSize" );

    printf("\nSize of matrix \"two_by_three_matrix\" is %1dx%1d.\n\n", nRows, nCols );

    array = (double*) malloc( sizeof(double) * nRows * nCols );

    /* 
       Now get the matrix entries and store them in row first order into
       array. 
    */
    error = tixiGetFloatMatrix( handle, "/plane/two_by_three_matrix", "blub", "bla", 
				nRows, nCols, ROW_WISE, array );

    printf("Matrix entries row wise storage: \n\n");
    printf("%3s    %3s\n", "Array", "Matrix" );
    printf("%3s  %3s  %3s\n", "Index", "I", "J" );

    for ( i = 0; i < nRows; i++ ) {
      for ( j = 0; j < nCols; j++ ) {
	printf("%3d    %3d  %3d %10.4f\n", i*nCols+j, i+1, j+1, array[i*nCols+j] );
      }
    }

    /* 
       Now get the matrix entries and store them in column first order into
       array. 
    */
    error = tixiGetFloatMatrix( handle, "/plane/two_by_three_matrix", "blub", "bla", 
				nRows, nCols, COLUMN_WISE, array );

    printf("\nMatrix entries column wise storage: \n\n");
    printf("%3s    %3s\n", "Array", "Matrix" );
    printf("%3s  %3s  %3s\n", "Index", "I", "J" );

    for ( j = 0; j < nCols; j++ ) {
      for ( i = 0; i < nRows; i++ ) {
   	printf("%3d    %3d  %3d %10.4f\n", j*nRows+i, i+1, j+1, array[j*nRows+i] );
      }
    }
  }

  /*
    Check elements
  */

  if ( tixiCheckElement( handle, "/plane/two_by_three_matrix" ) == SUCCESS ) {
    printf("\n Test passed: Element \"/plane/two_by_three_matrix\" exists.\n" );
  }
  else {
    printf("\n Test failed: Element \"/plane/two_by_three_matrix\" does not exist.\n" );
  }

  if ( tixiCheckElement( handle, "/plane/MATRIXX" ) != SUCCESS ) {
    printf("\n Test passed: Element \"/plane/MATRIXX\" does not exist.\n" );
  }
  else {
    printf("\n Test failed: Element \"/plane/MATRIXX\" exists.\n" );
  }


  /*
    Check if handle is  valid.
   */

  if ( tixiCheckDocumentHandle( handle ) ){
    printf("\n Test failed: Invalid document handle.\n" );
  }
  else {
    printf("\n Test passed: Document handle is valid.\n" );
  }
 
  /* 

     Now we close the document.All memory allocated internally by
     actions on this document is released. This means you have to make
     a copy of all strings returned by TIXI if you want to use them
     after closing the document. 
  */

  
  error = tixiCloseDocument( handle );
  CHECK_ERROR( error, "tixiCloseDocument" );

  /*
    Check the handle. It should be invalid after closing the document.
  */

   if ( tixiCheckDocumentHandle( handle ) ) {
     printf("\n Test passed: Invalid document handle after closing the document.\n" );
   }
   else {
     printf("\n Test failed: Document handle is valid. ");
     printf("            This should not happen. The file has been closed already!! \n" );
   }
}

/**
   Demonstrates how to create a new document and adds some contents.
 */

void writeXML( char* outputFileName ) {

  TixiDocumentHandle handle;
  ReturnCode error;
  int iWing;
 
  /* 
     Create a new document for writing with root element named
     "plane". A file name is attributed to this document on saving.
  */

  error = tixiCreateDocument( "plane", &handle );
  CHECK_ERROR( error, "tixiCreateDocument" );

  /* Any error occuring here is fatal! */
 
  if ( error != SUCCESS ) {
    abort();
  }
   
  printf("\nCreated document for writing.\n");

  /* 
     First a header containing the tool name and version and the author
     of the file is added. A timestamp is added automatically, as well. 
  */

  error = tixiAddHeader( handle, "NoTool", "47.11", "Me" );
  CHECK_ERROR( error, "tixiAddHeader" );

  {
    /*
      Now we insert a empty elements into the root element to create a
      hierarchy of elements.
    */
    
    int nWings = 6;
    
    error = tixiAddTextElement( handle, "/plane", "wings", NULL );
    CHECK_ERROR( error, "tixiAddTextElement" );
 
    printf("\nInserted empty wings element.\n");

    /* Insert empty wing elements into the wings element. */

    for ( iWing = 0; iWing < nWings; iWing++ ) {

      error = tixiAddTextElement( handle, "/plane/wings", "wing", NULL );
      CHECK_ERROR( error, "tixiAddTextElement" );

    }

    printf("\nInserted wing elements.\n\n");

    {
      /* 
	 Into each wing element insert a position attribute and a
	 wingspan element.
      */
  
      char* baseName = "/plane/wings/wing";
      int elementNameSize = 
	strlen(baseName)                       /* length of baseName            */
	+ ceil( log( (double) ( nWings + 1 ) ) ) /* number of digits of the index */
	+ 2                                      /* opening and closing brackets  */
	+ 1;                                     /* trailing "\0"                 */
 
      char* elementName = (char*) malloc( sizeof(char) * elementNameSize );

      /* XPath indices start at 1 !! */
      for ( iWing = 1; iWing <= nWings; iWing++ ) {

	char* position = iWing%2 == 0 ? "left" : "right";
    
	double span;
    
	/* Create element name string */
	strcpy( elementName, baseName );
	sprintf( &(elementName[ strlen(baseName)]), "[%1d]", iWing );
    
	/* Add attribute */
	error = tixiAddTextAttribute( handle, elementName, "position", position );
	CHECK_ERROR( error, "tixiAddTextAttribute" );

	printf("Added position attribute with value %5s to element %s.\n", position, elementName );
     
	/* Add wingspan element. */
	span = iWing + 0.1;
	error = tixiAddDoubleElement( handle, elementName, "wingspan", span, "%5.1f" );
	CHECK_ERROR( error, "tixiAddDoubleElement" );
     
	printf("Added wingspan element with value %5.1f to element %s.\n", span, elementName );

      }
   
      free(elementName);
    }
  }
  {
    /* write a 4x3 matrix */

    int nRows = 4;
    int nCols = 3;
    double array[12];
    int i;
    int j; 

    
    for ( i = 0; i < nRows; i++ ) {
      for ( j = 0; j < nCols; j++ ) {
	array[i*nCols+j] = i * 10. + j;
      }
    }

    /* Write array to matrix in row order. */

    error = tixiAddFloatMatrix( handle, "/plane", "four_by_three", "rows", "columns", 
				nRows, nCols, ROW_WISE, array, "%5.1f" );
    CHECK_ERROR( error, "tixiAddFloatMatrix" );

    printf("\n4x3 Matrix written\n");
  
    /* Now write the array to the matrix in column order */

    error = tixiAddFloatMatrix( handle, "/plane", "four_by_three", "rows", "columns", 
				nRows, nCols, COLUMN_WISE, array, "%5.1f" );

    CHECK_ERROR( error, "tixiAddFloatMatrix" );
    printf("\n3x4 Matrix written\n");

    /* Write array to matrix in row order. */

    error = tixiAddFloatMatrix( handle, "/plane", "four_by_three", "rows", "columns", 
				nRows, nCols, ROW_WISE, array, "%5.1f" );
    CHECK_ERROR( error, "tixiAddFloatMatrix" );

  }

  {
    /* 
       Create an empty 3x3 matrix and fill the entries with a composite element.
    */

  
    int nRows = 3;
    int nCols = 3;
    int i;
    int j; 

    /* first create an empty matrix */
    error = tixiCreateMatrix( handle, "/plane", "composite", "r", "c", nRows, nCols );
    CHECK_ERROR( error, "tixiCreateMatrix" );

    /* 
       Path to a matrix entry is for example
       /plane/composite/r[2]/c[3]
    */
    
    char* basePath = "/plane/composite/r";
    int basePathLength = strlen(basePath);

    for ( i = 1; i <= nRows; i++ ) {
      
      /* build string for i-th the row element path */

      int rowPathSize = 
	basePathLength +                      /* length of base string */
	ceil( log( (double) ( i + 1 ) ) + .1 ) +   /* number of digits of the index string */
	2 +                                   /* opening and closing brackets  */
	1;                                    /* trailing "\0"                 */
      int rowPathLength;
      
      char* rowPath = (char*) malloc( sizeof(char) * rowPathSize );
  
      strcpy( rowPath, basePath );
      sprintf( &(rowPath[basePathLength]), "[%1d]", i );
      rowPathLength = strlen(rowPath);

      for ( j = 1; j <= nCols; j++ ) {

	/* build string for the path to matrix  entry (i,j) */

	int entryNameSize = 
	  rowPathLength +                        /* length of row element string  */
	  2 +                                    /* length "/c"                   */
	  ceil( log( (double) (  j + 1 ) ) ) +   /* number of digits of the index */
	  2 +                                    /* opening and closing brackets  */
	  1;                                     /* trailing "\0"                 */

	char* entryName = (char*) malloc( sizeof(char) * entryNameSize );
	char* wingPath =  (char*) malloc( sizeof(char) * entryNameSize + 5 );
	char* position = (i*j)%2 == 0 ? "left" : "right";

	strcpy( entryName, rowPath );
	sprintf( &(entryName[rowPathLength]), "/c[%1d]", j );

	/* Add an empty wing element to the matrix element (i,j) */
	error = tixiAddTextElement( handle, entryName, "wing", NULL );
	CHECK_ERROR( error, "tixiAddTextElement" );
	
	strcpy( wingPath, entryName );
	strcat( wingPath, "/wing" );

	/* Insert a position and a span element into the wing element */
	error = tixiAddTextElement( handle, wingPath, "position", position );
	CHECK_ERROR( error, "tixiAddTextElement" );

	error = tixiAddDoubleElement( handle, wingPath, "span", (i+j) * 10., NULL );
	CHECK_ERROR( error, "tixiAddDoubleElement" );

	free(entryName);
	free(wingPath);
      }
      
      free( rowPath );
    }

   
  }	
 
  /* After all elements have been added, save the document to file */

  error = tixiSaveDocument( handle, outputFileName );
  CHECK_ERROR( error, "tixiSaveDocument" );
 
  /* Now we can close the document */

  error = tixiCloseDocument( handle );
  CHECK_ERROR( error, "tixiCloseDocument" );

  printf("\nDocument %s written.\n", outputFileName );

}

int main( int argc, char** argv ){

    char* xmlOutputFilename = "howtoout.xml";
    char* xmlInputFilename = "howtoin.xml";

    readXML( xmlInputFilename );
    writeXML( xmlOutputFilename );

    return (0);
}


