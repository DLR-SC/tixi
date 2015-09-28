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

    tixiCleanup();

    return (0);
}


