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
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "tixi.h"
/**
   @file Demo program to read and write XML files relatd to IBUCK.
*/

/* structure to hold panel data */
typedef struct
{
  double radius;
  double length;
  double width;
  double thickness;
  double emod;
  double nu;
} Panel;

/* structure to hold web data */
typedef struct
{
  double height;
  double thickness;
  double emod;
  double nu;
} Web;

typedef struct
{
  double height;
  double thickness;
} Flange;

typedef struct
{
  double width;
  double thickness;
} Doubler;

/* structure to hold stringer data */

typedef struct
{
  Web aWeb;
  Flange aFlange;
  Doubler aDoubler;
} Stringer;

typedef struct
{
  Web aWeb;
  Flange aFlange;
  Doubler aDoubler;
} Frame;


/**
   Routine to construct a new path by concatenating two XPath expressions 
   The memory pointed to by the returned pointer has to be freed by the user.
   
   @param parentPath (in) first part of the resulting string
   @param elementPath (in) second part of the resulting string
   @param outputPath (out) concatenation of parentPath and elementPath
 */
char *buildPath(char **outputPath, char *parentPath, char *elementPath)
{

  if (*outputPath) {
    if (strlen(parentPath) + strlen(elementPath) + 1 > strlen(*outputPath)) {
      size_t size = sizeof(char) * (strlen(parentPath) + strlen(elementPath) + 2);
      *outputPath = (char *) realloc((char *) *outputPath, size);
    }
  }
  else {
    size_t size = sizeof(char) * (strlen(parentPath) + strlen(elementPath) + 2);
    *outputPath = (char *) malloc(size);
  }


  strcpy(*outputPath, parentPath);
  strcat(*outputPath, "/");
  strcat(*outputPath, elementPath);

  return *outputPath;
}

void getPanelData(TixiDocumentHandle handle, char *panelPath, Panel * aPanel)
{

  char *elementPath = NULL;

  buildPath(&elementPath, panelPath, "radius");
  tixiGetDoubleElement(handle, elementPath, &(aPanel->radius));

  buildPath(&elementPath, panelPath, "length");
  tixiGetDoubleElement(handle, elementPath, &(aPanel->length));

  buildPath(&elementPath, panelPath, "width");
  tixiGetDoubleElement(handle, elementPath, &(aPanel->width));

  buildPath(&elementPath, panelPath, "thickness");
  tixiGetDoubleElement(handle, elementPath, &(aPanel->thickness));

  buildPath(&elementPath, panelPath, "emod");
  tixiGetDoubleElement(handle, elementPath, &(aPanel->emod));

  buildPath(&elementPath, panelPath, "nu");
  tixiGetDoubleElement(handle, elementPath, &(aPanel->nu));

  free(elementPath);
}

void getWebData(TixiDocumentHandle handle, char *basePath, Web * aWeb)
{

  char *elementPath = NULL;
  char *webPath = (char *) malloc(sizeof(char) * (strlen(basePath) + strlen("/web") + 1));

  strcpy(webPath, basePath);
  strcat(webPath, "/web");

  buildPath(&elementPath, webPath, "height");
  tixiGetDoubleElement(handle, elementPath, &(aWeb->height));

  buildPath(&elementPath, webPath, "thickness");
  tixiGetDoubleElement(handle, elementPath, &(aWeb->thickness));

  buildPath(&elementPath, webPath, "emod");
  tixiGetDoubleElement(handle, elementPath, &(aWeb->emod));

  buildPath(&elementPath, webPath, "nu");
  tixiGetDoubleElement(handle, elementPath, &(aWeb->nu));

  free(webPath);
  free(elementPath);
}

void getDoublerData(TixiDocumentHandle handle, char *basePath, Doubler * aDoubler)
{

  char *elementPath = NULL;

  char *doublerPath = (char *) malloc(sizeof(char) * (strlen(basePath) + strlen("/doubler") + 1));

  strcpy(doublerPath, basePath);
  strcat(doublerPath, "/doubler");

  buildPath(&elementPath, doublerPath, "width");
  tixiGetDoubleElement(handle, elementPath, &(aDoubler->width));

  buildPath(&elementPath, doublerPath, "thickness");
  tixiGetDoubleElement(handle, elementPath, &(aDoubler->thickness));

  free(doublerPath);
  free(elementPath);
}

void getFlangeData(TixiDocumentHandle handle, char *basePath, Flange * aFlange)
{

  char *elementPath = NULL;

  char *flangePath = (char *) malloc(sizeof(char) * (strlen(basePath) + strlen("/flange") + 1));

  strcpy(flangePath, basePath);
  strcat(flangePath, "/flange");

  buildPath(&elementPath, flangePath, "height");
  tixiGetDoubleElement(handle, elementPath, &(aFlange->height));

  buildPath(&elementPath, flangePath, "thickness");
  tixiGetDoubleElement(handle, elementPath, &(aFlange->thickness));

  free(flangePath);
  free(elementPath);
}

void readIBUCKInput(char *filename)
{

  TixiDocumentHandle handle;

  char *panelPath = "/toolInput/inputData/components/panel";
  char *stringerPath = "/toolInput/inputData/components/stringer";
  char *framePath = "/toolInput/inputData/components/frame";
  char *lengthUnit = NULL;
  char *pressureUnit = NULL;

  Panel aPanel;
  Stringer aStringer;
  Frame aFrame;
  
  ReturnCode ret = tixiOpenDocument(filename, &handle);
  assert(ret == SUCCESS);
  
  /* get panel data */

  getPanelData(handle, panelPath, &aPanel);

  /* get stringer data */

  getWebData(handle, stringerPath, &(aStringer.aWeb));
  getFlangeData(handle, stringerPath, &(aStringer.aFlange));
  getDoublerData(handle, stringerPath, &(aStringer.aDoubler));

  /* get frame data */

  getWebData(handle, framePath, &(aFrame.aWeb));
  getFlangeData(handle, framePath, &(aFrame.aFlange));
  getDoublerData(handle, framePath, &(aFrame.aDoubler));

  /* get length and pressure units */

  tixiGetTextElement(handle, "/toolInput/inputData/components/lengthUnit", &lengthUnit);
  tixiGetTextElement(handle, "/toolInput/inputData/components/pressureUnit", &pressureUnit);


  printf("\nPanel:\n");
  printf("    radius:        %15g %-2s\n", aPanel.radius, lengthUnit);
  printf("    length:        %15g %-2s\n", aPanel.length, lengthUnit);
  printf("    width:         %15g %-2s\n", aPanel.width, lengthUnit);
  printf("    thickness:     %15g %-2s\n", aPanel.thickness, lengthUnit);
  printf("    emod:          %15g %-3s\n", aPanel.emod, pressureUnit);
  printf("    nu:            %15g %-3s\n", aPanel.nu, pressureUnit);

  printf("\nStringer:\n");
  printf("    web:\n");
  printf("        height:    %15g %-2s\n", aStringer.aWeb.height, lengthUnit);
  printf("        thickness: %15g %-2s\n", aStringer.aWeb.thickness, lengthUnit);
  printf("        emod:      %15g %-3s\n", aStringer.aWeb.emod, pressureUnit);
  printf("        nu:        %15g %-3s\n", aStringer.aWeb.nu, pressureUnit);
  printf("    flange:\n");
  printf("        height:    %15g %-2s\n", aStringer.aFlange.height, lengthUnit);
  printf("        thickness: %15g %-2s\n", aStringer.aFlange.thickness, lengthUnit);
  printf("    doubler:\n");
  printf("        width:     %15g %-2s\n", aStringer.aDoubler.width, lengthUnit);
  printf("        thickness: %15g %-2s\n", aStringer.aDoubler.thickness, lengthUnit);

  printf("\nFrame:\n");
  printf("    web:\n");
  printf("        height:    %15g %-2s\n", aFrame.aWeb.height, lengthUnit);
  printf("        thickness: %15g %-2s\n", aFrame.aWeb.thickness, lengthUnit);
  printf("        emod:      %15g %-3s\n", aFrame.aWeb.emod, pressureUnit);
  printf("        nu:        %15g %-3s\n", aFrame.aWeb.nu, pressureUnit);
  printf("    flange:\n");
  printf("        height:    %15g %-2s\n", aFrame.aFlange.height, lengthUnit);
  printf("        thickness: %15g %-2s\n", aFrame.aFlange.thickness, lengthUnit);
  printf("    doubler:\n");
  printf("        width:     %15g %-2s\n", aFrame.aDoubler.width, lengthUnit);


  tixiCloseDocument(handle);
}

void outputIBUCKResult(char *outputFilename)
{
#define N_EIGENVALUES 5

  TixiDocumentHandle handle;

  double eigenvalues[N_EIGENVALUES] = {
    -0.14568026,
    0.16000421,
    0.16000421,
    -0.17728937,
    -0.25900444
  };

  char *types[N_EIGENVALUES] = {
    "local",
    "local",
    "local",
    "local",
    "local"
  };

  double additionalValue = -0.41672823;

  tixiCreateDocument("toolOutput", &handle);

  tixiAddHeader(handle, "IBUCK", "1.0", "Hans-Peter Kersken");

  /* Add empty text element. Into this element the output data are inserted */

  tixiAddTextElement(handle, "/toolOutput", "outputData", NULL);

  /* Add list of elements holding doubles with attributes */

  tixiAddDoubleListWithAttributes(handle, "/toolOutput/outputData",
                                  "eigenvals", "eigenval", "type",
                                  eigenvalues, "%g", types, N_EIGENVALUES);

  /* Append additional eigenvalue to the list. */

  tixiAddDoubleElement(handle, "/toolOutput/outputData/eigenvals", "eigenval", additionalValue,
                       "%g");

  /* Add type attribute to this element. We know it is the 6th element with name eigenval in eigenvals!! */

  tixiAddTextAttribute(handle, "/toolOutput/outputData/eigenvals/eigenval[6]", "type", "local");

  /* Finally, save the document to a file and close the document */

  tixiSaveDocument(handle, outputFilename);
  tixiCloseDocument(handle);

#undef N_EIGENVALUES
}

int main(int argc, char **argv)
{

  char *xmlInputFilename = "howtoin.xml";
  char *docString = NULL;
  double * myFloat = NULL;
  int i = 0;
  TixiDocumentHandle handle;

  printf("TIXI Demo\n\n");
  printf("Using TIXI %s\n", tixiGetVersion());

  //create xml doc
  tixiCreateDocument("MyRoot", &handle);
  tixiAddHeader(handle, "tixiDemo", tixiGetVersion(), "Martin Siggel");
  tixiAddIntegerElement(handle ,"/MyRoot", "MyInt", 1234, "%d");

  //add float vector
  myFloat = (double*) malloc(sizeof(double)*20);
  for(i = 0; i < 20; ++i) myFloat[i] = (double)i*i;
  tixiAddFloatVector(handle, "/MyRoot","MyFloatVector", myFloat, 20);
  free(myFloat);

  //print doc
  tixiExportDocumentAsString(handle, &docString);
  printf("%s", docString);

  //write to file
  tixiSaveDocument(handle, "MyFile.xml");
  tixiCloseDocument(handle);
  handle = 0;


  if(tixiOpenDocument(xmlInputFilename, &handle) != SUCCESS){
      printf("File could not be opened!\n");
      return 1;
  }
  tixiCreateElementAtIndex(handle, "/plane", "numberOfPassengers", 2);
  tixiSaveDocument( handle, "out.xml" );
  tixiCloseDocument(handle);
  tixiCleanup();

  return (0);
}
