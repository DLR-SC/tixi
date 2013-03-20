/* 
* Copyright (C) 2007-2012 German Aerospace Center (DLR/SC)
*
* Created: 2012-08-13 Martin Siggel <martin.siggel@dlr.de>
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

/* Visual Studio C macros */
#ifdef _MSC_VER
  #define _CRT_SECURE_NO_WARNINGS
  #define snprintf _snprintf
#endif

#include <math.h>
#include <matrix.h>
#include <mex.h>
#include <tixi.h>

#include <string.h>
#include <stdio.h>

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(WIN32)
#define MEX_EXPORT __declspec(dllexport)
#endif

/*
 * helping function to convert matlab array into c-string 
 */
int mxToString(const mxArray * mxStr, char ** cstr){
  size_t strLen = 0;

       /* Input must be a string. */
  if (mxIsChar(mxStr) != 1){
    mexErrMsgTxt("Input must be a string.");
    return 1;
  }

  /* Input must be a row vector. */
  if (mxGetM(mxStr) != 1){
    mexErrMsgTxt("Input must be a row vector.");
    return 1;
  }
    
  /* Get the length of the input string. */
  strLen = mxGetNumberOfElements(mxStr) + 1;

  /* Allocate memory for input and output strings. */
  *cstr = (char*) mxCalloc(strLen, sizeof(char));

  /* Copy the string data from prhs[0] into a C string 
   * input_buf. */
  return mxGetString(mxStr, *cstr, strLen);
}


void handleTixiError(ReturnCode code){
    switch(code){
case SUCCESS:
        break;
case FAILED:
        mexErrMsgTxt("Unspecified error\n");
        break;
case INVALID_XML_NAME:
        mexErrMsgTxt("Invalid xml file name\n");
        break;
case NOT_WELL_FORMED:
        mexErrMsgTxt("Document is not wellformed\n");
        break;
case NOT_SCHEMA_COMPLIANT:
        mexErrMsgTxt("Document is not schema compliant\n");
        break;
case NOT_DTD_COMPLIANT:
        mexErrMsgTxt("Document is not DTD compliant\n");
        break;
case INVALID_HANDLE:
        mexErrMsgTxt("Document handle is not valid\n");
        break;
case INVALID_XPATH:
        mexErrMsgTxt("XPath expression is not valid");
        break;
case ELEMENT_NOT_FOUND:
        mexErrMsgTxt("Element does not exist in document");
        break;
case INDEX_OUT_OF_RANGE:
        mexErrMsgTxt("Index supplied as argument is not inside the admissible range\n");
        break;
case NO_POINT_FOUND:
        mexErrMsgTxt("No point element found a given XPath\n");
        break;
case NOT_AN_ELEMENT:
        mexErrMsgTxt("XPath expression does not point to an XML-element node");
        break;
case ATTRIBUTE_NOT_FOUND:
        mexErrMsgTxt("Element does not have the attribute\n");
        break;
case OPEN_FAILED:
        mexErrMsgTxt("Error on opening the file\n");
        break;
case OPEN_SCHEMA_FAILED:
        mexErrMsgTxt(" Error on opening the schema file\n");
        break;
case OPEN_DTD_FAILED:
        mexErrMsgTxt("Error on opening the DTD file\n");
        break;
case CLOSE_FAILED:
        mexErrMsgTxt("Error on closing the file\n");
        break;
case ALREADY_SAVED:
        mexErrMsgTxt("Trying to modify already saved document\n");
        break;
case ELEMENT_PATH_NOT_UNIQUE:
        mexErrMsgTxt("Path expression can not be resolved unambiguously\n");
        break;
case NO_ELEMENT_NAME:
        mexErrMsgTxt("Element name argument is NULL\n");
        break;
case NO_CHILDREN:
        mexErrMsgTxt("Node has no children\n");
        break;
case CHILD_NOT_FOUND:
        mexErrMsgTxt("Named child is not child of element specified\n");
        break;
case EROROR_CREATE_ROOT_NODE:
        mexErrMsgTxt("Error when adding root node to new document\n");
        break;
case DEALLOCATION_FAILED:
        mexErrMsgTxt("On closing a document the deallocation of allocated memory fails\n");
        break;
case NO_NUMBER:
        mexErrMsgTxt("No number specified\n");
        break;
case NO_ATTRIBUTE_NAME:
        mexErrMsgTxt("No attribute name specified\n");
        break;
case STRING_TRUNCATED:
        mexErrMsgTxt("String variable supplied is to small to hold the result\n");
        break;
case NON_MATCHING_NAME:
        mexErrMsgTxt("Row or column name specified do not match the names used in the document\n");
        break;
case NON_MATCHING_SIZE:
        mexErrMsgTxt("Number of rows or columns specified do not match the sizes of the matrix in the document\n");
        break;
case MATRIX_DIMENSION_ERROR:
        mexErrMsgTxt("Supplied matrix dimensions are smaller than 1\n");
        break;
case COORDINATE_NOT_FOUND:
        mexErrMsgTxt("Missing coordinate inside a point element\n");
        break;
case UNKNOWN_STORAGE_MODE:
        mexErrMsgTxt("Storage mode specified is neither ROW_WISE nor COLUMN_WISE\n");
        break;
case UID_NOT_UNIQUE:
        mexErrMsgTxt("One or more uID's are not unique\n");
        break;
case UID_DONT_EXISTS:
        mexErrMsgTxt("A given uID does not exist\n");
        break;
case UID_LINK_BROKEN:
        mexErrMsgTxt("A specified Link has no correspoding uid in that data set\n");
        break;
default:
        mexErrMsgTxt("An unknown error occured\n");
    }
}

int mxToInt(const mxArray * mxArr){
    double dv = *mxGetPr(mxArr);
    if(!isinteger(dv))
        mexWarnMsgTxt("Argument is not an integer\n");

    return (int)dv;
}

void mex_tixiGetVersion(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    plhs[0] = mxCreateString(tixiGetVersion());
}

void mex_tixiAddHeader(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    char * toolName = NULL;
    char * version = NULL;
    char * authorName = NULL;
    TixiDocumentHandle handle = 0;

    if(nrhs != 5){
        mexErrMsgTxt("tixiAddHeader(handle, toolName, version, authorName): Wrong number of arguments\n");
    }

    if(!isscalar(prhs[1])){
        mexErrMsgTxt("Invalid Handle!\n");
    }

    if(!mxIsChar(prhs[2]))
        mexErrMsgTxt("Invalid toolName argument\n");

    if(!mxIsChar(prhs[3]))
        mexErrMsgTxt("Invalid version argument\n");

    if(!mxIsChar(prhs[4]))
        mexErrMsgTxt("Invalid authorName argument\n");

    handle = mxToInt(prhs[1]);
    mxToString(prhs[2],&toolName);
    mxToString(prhs[3],&version);
    mxToString(prhs[4],&authorName);

    handleTixiError(tixiAddHeader(handle, toolName, version, authorName));
}

void mex_tixiAddCpacsHeader(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    char * name = NULL;
    char * version = NULL;
    char * creator = NULL;
    char * description = NULL;
    char * cpacsVersion = NULL;
    TixiDocumentHandle handle = 0;

    if(nrhs != 7)
        mexErrMsgTxt("tixiAddCpacsHeader(handle, name, creator, version, description): Wrong number of arguments\n");

    if(!isscalar(prhs[1]))
        mexErrMsgTxt("Invalid Handle!\n");

    if(!mxIsChar(prhs[2]))
        mexErrMsgTxt("Invalid name argument\n");

    if(!mxIsChar(prhs[3]))
        mexErrMsgTxt("Invalid creator argument\n");

    if(!mxIsChar(prhs[4]))
        mexErrMsgTxt("Invalid version argument\n");

    if(!mxIsChar(prhs[5]))
        mexErrMsgTxt("Invalid description argument\n");

    if(!mxIsChar(prhs[6]))
        mexErrMsgTxt("Invalid cpacs version argument\n");


    handle = mxToInt(prhs[1]);
    mxToString(prhs[2],&name);
    mxToString(prhs[3],&creator);
    mxToString(prhs[4],&version);
    mxToString(prhs[5],&description);
    mxToString(prhs[6],&cpacsVersion);


    handleTixiError(tixiAddCpacsHeader(handle, name, creator, version, description, cpacsVersion));
}

void mex_tixiOpenDocument(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    char * filename = NULL;
    TixiDocumentHandle handle;

    if(nrhs != 2){
        mexErrMsgTxt("tixiOpenDocument(filename): Wrong number of arguments\n");
    }

    // get the filename
    mxToString(prhs[1],&filename);
    handleTixiError(tixiOpenDocument(filename, &handle));

    // return the handle
    plhs[0] = mxCreateDoubleMatrix(1,1, mxREAL);
    *mxGetPr(plhs[0]) = handle;
}

void mex_tixiOpenDocumentRecursive(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    char * filename = NULL;
    OpenMode mode = OPENMODE_PLAIN;
    TixiDocumentHandle handle;

    if(nrhs != 3){
        mexErrMsgTxt("tixiOpenDocument(filename, openMode): Wrong number of arguments\n");
    }

    // get the filename
    mxToString(prhs[1],&filename);

    // get openmode
    mode = (OpenMode) mxToInt(prhs[2]);
    if(mode != OPENMODE_PLAIN && mode != OPENMODE_RECURSIVE){
        mexErrMsgTxt("Invalid open mode\n");
    }

    handleTixiError(tixiOpenDocumentRecursive(filename, &handle, mode));

    // return the handle
    plhs[0] = mxCreateDoubleMatrix(1,1, mxREAL);
    *mxGetPr(plhs[0]) = handle;
}

void mex_tixiOpenDocumentFromHTTP(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    char * url = NULL;
    TixiDocumentHandle handle;

    if(nrhs != 2){
        mexErrMsgTxt("tixiOpenDocumentFromHTTP(filename): Wrong number of arguments\n");
    }

    // get the url
    mxToString(prhs[1],&url);
    handleTixiError(tixiOpenDocumentFromHTTP(url, &handle));

    // return the handle
    plhs[0] = mxCreateDoubleMatrix(1,1, mxREAL);
    *mxGetPr(plhs[0]) = handle;
}

void mex_tixiCreateDocument(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    char * rootName = NULL;
    TixiDocumentHandle handle;

    if(nrhs != 2){
        mexErrMsgTxt("tixiCreateDocument(rootElementName): Wrong number of arguments\n");
    }

    // get the root name
    mxToString(prhs[1],&rootName);
    handleTixiError(tixiCreateDocument(rootName, &handle));

    // return the handle
    plhs[0] = mxCreateDoubleMatrix(1,1, mxREAL);
    *mxGetPr(plhs[0]) = handle;
}


void mex_tixiCloseDocument(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    const mxArray * mxHandle = prhs[1];
    int handle = -1;
    if(nrhs != 2){
        mexErrMsgTxt("tixiCloseDocument(handle): Wrong number of arguments\n");
    }

    if(!isscalar(mxHandle)){
        mexErrMsgTxt("Wrong handle format\n");
    }

    // get handle
    handle = (int) *mxGetPr(mxHandle);
    handleTixiError(tixiCloseDocument(handle));
}

void mex_tixiCloseAllDocuments(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    handleTixiError(tixiCloseAllDocuments());
}

void mex_tixiSchemaValidateFromFile(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    char * xsdFilename = NULL;
    int handle = -1;
    if(nrhs != 3){
        mexErrMsgTxt("tixiSchemaValidateFromFile(handle, xsdFilename): Wrong number of arguments\n");
    }

    if(!isscalar(prhs[1])){
        mexErrMsgTxt("Invalid Handle!\n");
    }

    if(!mxIsChar(prhs[2]))
        mexErrMsgTxt("Invalid xdsFilename argument\n");


    handle = mxToInt(prhs[1]);
    mxToString(prhs[2],&xsdFilename);
    handleTixiError(tixiSchemaValidateFromFile(handle, xsdFilename));
}

void mex_tixiSchemaValidateFromString(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    char * string = NULL;
    int handle = -1;
    if(nrhs != 3){
        mexErrMsgTxt("tixiSchemaValidateFromString(handle, string): Wrong number of arguments\n");
    }

    if(!isscalar(prhs[1])){
        mexErrMsgTxt("Invalid Handle!\n");
    }

    if(!mxIsChar(prhs[2]))
        mexErrMsgTxt("Invalid string argument\n");


    handle = mxToInt(prhs[1]);
    mxToString(prhs[2],&string);
    handleTixiError(tixiSchemaValidateFromString(handle, string));
}

void mex_tixiDTDValidate(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    char * DTDFilename = NULL;
    int handle = -1;
    if(nrhs != 3){
        mexErrMsgTxt("tixiDTDValidate(handle, DTDFilename): Wrong number of arguments\n");
    }

    if(!isscalar(prhs[1])){
        mexErrMsgTxt("Invalid Handle!\n");
    }

    if(!mxIsChar(prhs[2]))
        mexErrMsgTxt("Invalid DTDFilename argument\n");


    handle = mxToInt(prhs[1]);
    mxToString(prhs[2],&DTDFilename);
    handleTixiError(tixiDTDValidate(handle, DTDFilename));
}


void mex_tixiGetTextElement(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    char * xpath = NULL;
    int handle = -1;
    char * text;
    if(nrhs != 3){
        mexErrMsgTxt("tixiGetTextElement(handle, xpath): Wrong number of arguments\n");
    }

    if(!isscalar(prhs[1])){
        mexErrMsgTxt("Invalid Handle!\n");
    }

    if(!mxIsChar(prhs[2]))
        mexErrMsgTxt("Invalid xpath argument\n");


    handle = mxToInt(prhs[1]);
    mxToString(prhs[2],&xpath);
    handleTixiError(tixiGetTextElement(handle, xpath, &text));

    plhs[0] = mxCreateString(text);
}

void mex_tixiGetDoubleElement(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    char * xpath = NULL;
    int handle = -1;
    double d;
    if(nrhs != 3){
        mexErrMsgTxt("tixiGetDoubleElement(handle, xpath): Wrong number of arguments\n");
    }

    if(!isscalar(prhs[1])){
        mexErrMsgTxt("Invalid Handle!\n");
    }

    if(!mxIsChar(prhs[2]))
        mexErrMsgTxt("Invalid xpath argument\n");


    handle = mxToInt(prhs[1]);
    mxToString(prhs[2],&xpath);
    handleTixiError(tixiGetDoubleElement(handle, xpath, &d));

    plhs[0] = mxCreateDoubleMatrix(1,1, mxREAL);
    *mxGetPr(plhs[0]) = d;
}

void mex_tixiGetIntegerElement(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    char * xpath = NULL;
    int handle = -1;
    int i;
    if(nrhs != 3){
        mexErrMsgTxt("tixiGetIntegerElement(handle, xpath): Wrong number of arguments\n");
    }

    if(!isscalar(prhs[1])){
        mexErrMsgTxt("Invalid Handle!\n");
    }

    if(!mxIsChar(prhs[2]))
        mexErrMsgTxt("Invalid xpath argument\n");


    handle = mxToInt(prhs[1]);
    mxToString(prhs[2],&xpath);
    handleTixiError(tixiGetIntegerElement(handle, xpath, &i));

    plhs[0] = mxCreateDoubleMatrix(1,1, mxREAL);
    *mxGetPr(plhs[0]) = (double)i;
}

void mex_tixiGetBooleanElement(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    char * xpath = NULL;
    int handle = -1;
    int b = 0;
    if(nrhs != 3){
        mexErrMsgTxt("tixiGetBooleanElement(handle, xpath): Wrong number of arguments\n");
    }

    if(!isscalar(prhs[1])){
        mexErrMsgTxt("Invalid Handle!\n");
    }

    if(!mxIsChar(prhs[2]))
        mexErrMsgTxt("Invalid xpath argument\n");


    handle = mxToInt(prhs[1]);
    mxToString(prhs[2],&xpath);
    handleTixiError(tixiGetBooleanElement(handle, xpath, &b));

    plhs[0] = mxCreateLogicalScalar(b);
}

void mex_tixiUpdateTextElement(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    char * xpath = NULL;
    char * text = NULL;
    int handle = -1;
    if(nrhs != 4){
        mexErrMsgTxt("tixiUpdateTextElement(handle, xpath, text): Wrong number of arguments\n");
    }

    if(!isscalar(prhs[1])){
        mexErrMsgTxt("Invalid Handle!\n");
    }

    if(!mxIsChar(prhs[2]))
        mexErrMsgTxt("Invalid xpath argument\n");

    if(!mxIsChar(prhs[3]))
        mexErrMsgTxt("Invalid text argument\n");


    handle = mxToInt(prhs[1]);
    mxToString(prhs[2],&xpath);
    mxToString(prhs[3],&text);
    handleTixiError(tixiUpdateTextElement(handle, xpath, text));
}

void mex_tixiUpdateDoubleElement(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    char * xpath = NULL;
    char * format = NULL;
    double d = 0;
    int handle = -1;
    if(nrhs != 5){
        mexErrMsgTxt("tixiUpdateDoubleElement(handle, xpath, value, format): Wrong number of arguments\n");
    }

    if(!isscalar(prhs[1])){
        mexErrMsgTxt("Invalid Handle!\n");
    }

    if(!mxIsChar(prhs[2]))
        mexErrMsgTxt("Invalid xpath argument\n");

    if(!isscalar(prhs[3]))
        mexErrMsgTxt("Invalid value argument\n");

    if(!mxIsChar(prhs[4]))
        mexErrMsgTxt("Invalid format argument\n");


    handle = mxToInt(prhs[1]);
    mxToString(prhs[2],&xpath);
    
    d = *mxGetPr(prhs[3]);

    mxToString(prhs[4],&format);
    handleTixiError(tixiUpdateDoubleElement(handle, xpath, d, format));
}

void mex_tixiUpdateIntegerElement(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    char * xpath = NULL;
    char * format = NULL;
    int d = 0;
    int handle = -1;
    if(nrhs != 5){
        mexErrMsgTxt("tixiUpdateIntegerElement(handle, xpath, value, format): Wrong number of arguments\n");
    }

    if(!isscalar(prhs[1])){
        mexErrMsgTxt("Invalid Handle!\n");
    }

    if(!mxIsChar(prhs[2]))
        mexErrMsgTxt("Invalid xpath argument\n");

    if(!isscalar(prhs[3]))
        mexErrMsgTxt("Invalid value argument\n");

    if(!mxIsChar(prhs[4]))
        mexErrMsgTxt("Invalid format argument\n");


    handle = mxToInt(prhs[1]);
    mxToString(prhs[2],&xpath);

    d = (int) *mxGetPr(prhs[3]);

    mxToString(prhs[4],&format);
    handleTixiError(tixiUpdateIntegerElement(handle, xpath, d, format));
}

void mex_tixiUpdateBooleanElement(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    char * xpath = NULL;
    int boolean = 0;
    int handle = -1;
    if(nrhs != 4){
        mexErrMsgTxt("tixiUpdateBooleanElement(handle, xpath, boolean): Wrong number of arguments\n");
    }

    if(!isscalar(prhs[1])){
        mexErrMsgTxt("Invalid Handle!\n");
    }

    if(!mxIsChar(prhs[2]))
        mexErrMsgTxt("Invalid xpath argument\n");

    if(mxIsChar(prhs[3]))
        mexErrMsgTxt("Invalid boolean argument\n");


    handle = mxToInt(prhs[1]);
    mxToString(prhs[2],&xpath);
    boolean = (int) *mxGetPr(prhs[3]);
    handleTixiError(tixiUpdateBooleanElement(handle, xpath, boolean));
}

void mex_tixiAddTextElement(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    char * xpath = NULL;
    char * name = NULL;
    char * text = NULL;
    int handle = -1;
    if(nrhs != 5){
        mexErrMsgTxt("tixiAddTextElement(handle, parentPath, elementName, text): Wrong number of arguments\n");
    }

    if(!isscalar(prhs[1])){
        mexErrMsgTxt("Invalid Handle!\n");
    }

    if(!mxIsChar(prhs[2]))
        mexErrMsgTxt("Invalid xpath argument\n");

    if(!mxIsChar(prhs[3]))
        mexErrMsgTxt("Invalid name argument\n");

    if(!mxIsChar(prhs[4]))
        mexErrMsgTxt("Invalid text argument\n");


    handle = mxToInt(prhs[1]);
    mxToString(prhs[2],&xpath);
    mxToString(prhs[3],&name);
    mxToString(prhs[4],&text);
    handleTixiError(tixiAddTextElement(handle, xpath, name, text));
}

void mex_tixiAddBooleanElement(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    char * xpath = NULL;
    char * name = NULL;
    int b = 0;
    int handle = -1;
    if(nrhs != 5){
        mexErrMsgTxt("tixiAddBooleanElement(handle, parentPath, elementName, boolean): Wrong number of arguments\n");
    }

    if(!isscalar(prhs[1])){
        mexErrMsgTxt("Invalid Handle!\n");
    }

    if(!mxIsChar(prhs[2]))
        mexErrMsgTxt("Invalid xpath argument\n");

    if(!mxIsChar(prhs[3]))
        mexErrMsgTxt("Invalid name argument\n");

    if(!isscalar(prhs[4]) || !mxIsLogical(prhs[4]))
        mexErrMsgTxt("Invalid boolean argument\n");


    handle = mxToInt(prhs[1]);
    mxToString(prhs[2],&xpath);
    mxToString(prhs[3],&name);
    b = *mxGetLogicals(prhs[4]);
    handleTixiError(tixiAddBooleanElement(handle, xpath, name, b));
    mxFree(xpath);
    mxFree(name);
}

void mex_tixiAddDoubleElement(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    char * xpath = NULL;
    char * name = NULL;
    char * format = NULL;
    double d = 0.;
    int handle = -1;
    if(nrhs != 6){
        mexErrMsgTxt("tixiAddDoubleElement(handle, parentPath, elementName, dvalue, format): Wrong number of arguments\n");
    }

    if(!isscalar(prhs[1])){
        mexErrMsgTxt("Invalid Handle!\n");
    }

    if(!mxIsChar(prhs[2]))
        mexErrMsgTxt("Invalid xpath argument\n");

    if(!mxIsChar(prhs[3]))
        mexErrMsgTxt("Invalid name argument\n");

    if(!isscalar(prhs[4]) || mxIsChar(prhs[4]))
        mexErrMsgTxt("Invalid value argument\n");

    if(!mxIsChar(prhs[5]))
        mexErrMsgTxt("Invalid format argument\n");

    handle = mxToInt(prhs[1]);
    mxToString(prhs[2],&xpath);
    mxToString(prhs[3],&name);
    d = *mxGetPr(prhs[4]);
    mxToString(prhs[5],&format);
    handleTixiError(tixiAddDoubleElement(handle, xpath, name, d, format));
    mxFree(format);
    mxFree(xpath);
    mxFree(name);
}

void mex_tixiAddIntegerElement(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    char * xpath = NULL;
    char * name = NULL;
    char * format = NULL;
    int i = 0;
    int handle = -1;
    if(nrhs != 6){
        mexErrMsgTxt("tixiAddIntegerElement(handle, parentPath, elementName, ivalue, format): Wrong number of arguments\n");
    }

    if(!isscalar(prhs[1])){
        mexErrMsgTxt("Invalid Handle!\n");
    }

    if(!mxIsChar(prhs[2]))
        mexErrMsgTxt("Invalid xpath argument\n");

    if(!mxIsChar(prhs[3]))
        mexErrMsgTxt("Invalid name argument\n");

    if(!isscalar(prhs[4]) || mxIsChar(prhs[4]))
        mexErrMsgTxt("Invalid value argument\n");

    if(!mxIsChar(prhs[5]))
        mexErrMsgTxt("Invalid format argument\n");

    handle = mxToInt(prhs[1]);
    mxToString(prhs[2],&xpath);
    mxToString(prhs[3],&name);
    i = (int) *mxGetPr(prhs[4]);
    mxToString(prhs[5],&format);
    handleTixiError(tixiAddIntegerElement(handle, xpath, name, i, format));
    mxFree(format);
    mxFree(xpath);
    mxFree(name);
}

void mex_tixiAddTextElementAtIndex(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    char * xpath = NULL;
    char * name = NULL;
    char * text = NULL;
    int index = 0;
    int handle = -1;
    if(nrhs != 6){
        mexErrMsgTxt("tixiAddTextElementAtIndex(handle, parentPath, elementName, text, index): Wrong number of arguments\n");
    }

    if(!isscalar(prhs[1])){
        mexErrMsgTxt("Invalid Handle!\n");
    }

    if(!mxIsChar(prhs[2]))
        mexErrMsgTxt("Invalid xpath argument\n");

    if(!mxIsChar(prhs[3]))
        mexErrMsgTxt("Invalid name argument\n");

    if(!mxIsChar(prhs[4]))
        mexErrMsgTxt("Invalid text argument\n");

    if(!isscalar(prhs[5]))
        mexErrMsgTxt("Invalid index argument\n");

    handle = mxToInt(prhs[1]);
    mxToString(prhs[2],&xpath);
    mxToString(prhs[3],&name);
    mxToString(prhs[4],&text);
    index = mxToInt(prhs[5]);
    handleTixiError(tixiAddTextElementAtIndex(handle, xpath, name, text,index));
}


void mex_tixiSaveDocument(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    char * filename = NULL;
    int handle = -1;
    if(nrhs != 3){
        mexErrMsgTxt("tixiSaveDocument(handle, filename): Wrong number of arguments\n");
    }

    if(!isscalar(prhs[1]))
        mexErrMsgTxt("Invalid Handle!\n");

    if(!mxIsChar(prhs[2]))
        mexErrMsgTxt("Invalid filename argument\n");


    handle = mxToInt(prhs[1]);
    mxToString(prhs[2],&filename);
    handleTixiError(tixiSaveDocument(handle, filename));
}

void mex_tixiSaveCompleteDocument(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    char * filename = NULL;
    int handle = -1;
    if(nrhs != 3){
        mexErrMsgTxt("tixiSaveCompleteDocument(handle, filename): Wrong number of arguments\n");
    }

    if(!isscalar(prhs[1]))
        mexErrMsgTxt("Invalid Handle!\n");

    if(!mxIsChar(prhs[2]))
        mexErrMsgTxt("Invalid filename argument\n");


    handle = mxToInt(prhs[1]);
    mxToString(prhs[2],&filename);
    handleTixiError(tixiSaveCompleteDocument(handle, filename));
}

void mex_tixiSaveAndRemoveDocument(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    char * filename = NULL;
    int handle = -1;
    if(nrhs != 3){
        mexErrMsgTxt("tixiSaveAndRemoveDocument(handle, filename): Wrong number of arguments\n");
    }

    if(!isscalar(prhs[1]))
        mexErrMsgTxt("Invalid Handle!\n");

    if(!mxIsChar(prhs[2]))
        mexErrMsgTxt("Invalid filename argument\n");


    handle = mxToInt(prhs[1]);
    mxToString(prhs[2],&filename);
    handleTixiError(tixiSaveAndRemoveDocument(handle, filename));
}

void mex_tixiExportDocumentAsString(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    char * str = NULL;
    int handle = -1;
    if(nrhs != 2)
         mexErrMsgTxt("tixiExportDocumentAsString(handle): Wrong number of arguments\n");

    if(!isscalar(prhs[1]))
        mexErrMsgTxt("Invalid Handle!\n");

    handle = mxToInt(prhs[1]);
    handleTixiError(tixiExportDocumentAsString(handle, &str));

    plhs[0] = mxCreateString(str);
}


void mex_tixiImportFromString(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    char * string = NULL;
    TixiDocumentHandle handle;

    if(nrhs != 2){
        mexErrMsgTxt("tixiImportFromString(string): Wrong number of arguments\n");
    }

    // get the string
    mxToString(prhs[1],&string);
    handleTixiError(tixiImportFromString(string, &handle));

    // return the handle
    plhs[0] = mxCreateDoubleMatrix(1,1, mxREAL);
    *mxGetPr(plhs[0]) = handle;
    mxFree(string);
}

void mex_tixiGetVectorSize(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    char * path = NULL;
    int handle = 0;
    int size = 0;
    if(nrhs != 3){
        mexErrMsgTxt("tixiGetVectorSize(handle, vectorPath): Wrong number of arguments\n");
    }

    if(!isscalar(prhs[1]))
        mexErrMsgTxt("Invalid Handle!\n");

    if(!mxIsChar(prhs[2]))
        mexErrMsgTxt("Invalid vectorPath argument\n");

    handle = mxToInt(prhs[1]);
    mxToString(prhs[2],&path);
    handleTixiError(tixiGetVectorSize(handle, path, &size));

    // return the size
    plhs[0] = mxCreateDoubleMatrix(1,1, mxREAL);
    *mxGetPr(plhs[0]) = (double) size;
}

void mex_tixiCreateElement(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    char * xpath = NULL;
    char * name = NULL;
    int handle = -1;
    if(nrhs != 4){
        mexErrMsgTxt("tixiCreateElement(handle, parentPath, elementName): Wrong number of arguments\n");
    }

    if(!isscalar(prhs[1])){
        mexErrMsgTxt("Invalid Handle!\n");
    }

    if(!mxIsChar(prhs[2]))
        mexErrMsgTxt("Invalid xpath argument\n");

    if(!mxIsChar(prhs[3]))
        mexErrMsgTxt("Invalid element name argument\n");


    handle = mxToInt(prhs[1]);
    mxToString(prhs[2],&xpath);
    mxToString(prhs[3],&name);
    handleTixiError(tixiCreateElement(handle, xpath, name));
}


void mex_tixiCreateElementAtIndex(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    char * xpath = NULL;
    char * name = NULL;
    int index = 0;
    int handle = -1;
    if(nrhs != 5){
        mexErrMsgTxt("tixiCreateElementAtIndex(handle, parentPath, elementName, index): Wrong number of arguments\n");
    }

    if(!isscalar(prhs[1])){
        mexErrMsgTxt("Invalid Handle!\n");
    }

    if(!mxIsChar(prhs[2]))
        mexErrMsgTxt("Invalid xpath argument\n");

    if(!mxIsChar(prhs[3]))
        mexErrMsgTxt("Invalid name argument\n");

    if(!isscalar(prhs[4]))
        mexErrMsgTxt("Invalid index argument\n");

    handle = mxToInt(prhs[1]);
    mxToString(prhs[2],&xpath);
    mxToString(prhs[3],&name);
    index = mxToInt(prhs[4]);
    handleTixiError(tixiCreateElementAtIndex(handle, xpath, name,index));
}

void mex_tixiRemoveElement(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    char * xpath = NULL;
    int handle = -1;
    if(nrhs != 3){
        mexErrMsgTxt("tixiRemoveElement(handle, xpath): Wrong number of arguments\n");
    }

    if(!isscalar(prhs[1])){
        mexErrMsgTxt("Invalid Handle!\n");
    }

    if(!mxIsChar(prhs[2]))
        mexErrMsgTxt("Invalid xpath argument\n");


    handle = mxToInt(prhs[1]);
    mxToString(prhs[2],&xpath);
    handleTixiError(tixiRemoveElement(handle, xpath));
    mxFree(xpath);
}

void mex_tixiRemoveAttribute(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    char * xpath = NULL;
    char * name  = NULL;
    int handle = -1;
    if(nrhs != 4){
        mexErrMsgTxt("tixiRemoveAttribute(handle, xpath, name): Wrong number of arguments\n");
    }

    if(!isscalar(prhs[1])){
        mexErrMsgTxt("Invalid Handle!\n");
    }

    if(!mxIsChar(prhs[2]))
        mexErrMsgTxt("Invalid xpath argument\n");

    if(!mxIsChar(prhs[3]))
        mexErrMsgTxt("Invalid attribute name\n");


    handle = mxToInt(prhs[1]);
    mxToString(prhs[2],&xpath);
    mxToString(prhs[3],&name);
    handleTixiError(tixiRemoveAttribute(handle, xpath,name));
    mxFree(name);
    mxFree(xpath);
}

void mex_tixiGetFloatVector(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    char * path = NULL;
    int handle = 0;
    int size = 0;
    int i = 0;
    double * array = NULL;
    double * outArray = NULL;
    if(nrhs != 4){
        mexErrMsgTxt("tixiGetFloatVector(handle, vectorPath, numValues): Wrong number of arguments\n");
    }

    if(!isscalar(prhs[1]))
        mexErrMsgTxt("Invalid Handle!\n");

    if(!mxIsChar(prhs[2]))
        mexErrMsgTxt("Invalid vectorPath argument\n");

    if(!isscalar(prhs[3]))
        mexErrMsgTxt("Invalid vector size argument!\n");

    handle = mxToInt(prhs[1]);
    mxToString(prhs[2],&path);
    size = mxToInt(prhs[3]);

    if(size < 1)
        mexErrMsgTxt("Invalid number of elements\n");

    handleTixiError(tixiGetFloatVector(handle, path, &array, size));

    // return the vector
    plhs[0] = mxCreateDoubleMatrix(1,size, mxREAL);
    outArray = mxGetPr(plhs[0]);
    for(i = 0; i < size; ++i)
        outArray[i] = array[i];
}

void mex_tixiAddFloatVector(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    char * path = NULL;
    char * name = NULL;
    int handle = 0;
    size_t size = 0;
    const mxArray * mVec = NULL;
    if(nrhs != 5){
        mexErrMsgTxt("tixiAddFloatVector(handle, parentPath, vectorName, vector): Wrong number of arguments\n");
    }

    if(!isscalar(prhs[1]))
        mexErrMsgTxt("Invalid Handle!\n");

    if(!mxIsChar(prhs[2]))
        mexErrMsgTxt("Invalid parentPath argument\n");

    if(!mxIsChar(prhs[3]))
        mexErrMsgTxt("Invalid vectorName argument\n");

    if(mxIsChar(prhs[4]))
        mexErrMsgTxt("Invalid float vector\n");

    handle = mxToInt(prhs[1]);
    mxToString(prhs[2],&path);
    mxToString(prhs[3],&name);

    mVec = prhs[4];
    if(!isvector(mVec))
        mexErrMsgTxt("Dimension mismatch of input vector");

    size = mxGetNumberOfElements(mVec);

    if(size < 1)
        mexErrMsgTxt("Invalid number of elements\n");

    handleTixiError(tixiAddFloatVector(handle, path, name, mxGetPr(mVec), (int) size));
}


void mex_tixiGetNamedChildrenCount(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    char * xpath = NULL;
    char * childName = NULL;
    int handle = -1;
    int count = 0;
    if(nrhs != 4){
        mexErrMsgTxt("tixiGetNamedChildrenCount(handle, xpath, childName): Wrong number of arguments\n");
    }

    if(!isscalar(prhs[1])){
        mexErrMsgTxt("Invalid Handle!\n");
    }

    if(!mxIsChar(prhs[2]))
        mexErrMsgTxt("Invalid xpath argument\n");

    if(!mxIsChar(prhs[3]))
        mexErrMsgTxt("Invalid child name\n");


    handle = mxToInt(prhs[1]);
    mxToString(prhs[2],&xpath);
    mxToString(prhs[3],&childName);
    handleTixiError(tixiGetNamedChildrenCount(handle, xpath, childName, &count));

    plhs[0] = mxCreateDoubleMatrix(1,1, mxREAL);
    *mxGetPr(plhs[0]) = (double) count;
}

void mex_tixiAddPoint(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    char * xpath = NULL;
    char * format = NULL;
    double x = 0., y = 0., z = 0.;
    int handle = -1;
    const mxArray * mpoint = NULL;
    if(nrhs != 5){
        mexErrMsgTxt("tixiAddPoint(handle, parentPath, point, format): Wrong number of arguments\n");
    }

    if(!isscalar(prhs[1])){
        mexErrMsgTxt("Invalid Handle!\n");
    }

    if(!mxIsChar(prhs[2]))
        mexErrMsgTxt("Invalid parentPath argument\n");

    //check dimension of point (has to be 3)
    mpoint = prhs[3];
    if(!isvector(mpoint) || mxGetNumberOfElements(mpoint)!=3)
        mexErrMsgTxt("Point argument has to be 3-dimensional\n.");

    if(!mxIsChar(prhs[4]))
        mexErrMsgTxt("Invalid format argument\n");


    handle = mxToInt(prhs[1]);
    mxToString(prhs[2],&xpath);

    x = mxGetPr(mpoint)[0];
    y = mxGetPr(mpoint)[1];
    z = mxGetPr(mpoint)[2];

    mxToString(prhs[4],&format);
    handleTixiError(tixiAddPoint(handle, xpath, x, y, z, format));
}

void mex_tixiGetPoint(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    char * xpath = NULL;
    double x = 0., y = 0., z = 0.;
    int handle = -1;
    double * point = NULL;
    if(nrhs != 3){
        mexErrMsgTxt("tixiGetPoint(handle, path): Wrong number of arguments\n");
    }

    if(!isscalar(prhs[1])){
        mexErrMsgTxt("Invalid Handle!\n");
    }

    if(!mxIsChar(prhs[2]))
        mexErrMsgTxt("Invalid path argument\n");

    handle = mxToInt(prhs[1]);
    mxToString(prhs[2],&xpath);

    handleTixiError(tixiGetPoint(handle, xpath, &x, &y, &z));

    plhs[0] = mxCreateDoubleMatrix(1,3, mxREAL);
    
    point = mxGetPr(plhs[0]);
    point[0] = x; point[1] = y; point[2] = z;
}

void mex_tixiAddTextAttribute(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    char * xpath = NULL;
    char * name  = NULL;
    char * val   = NULL;
    int handle   = -1;

    if(nrhs != 5){
        mexErrMsgTxt("tixiAddTextAttribute(handle, path, attribName, attribValue): Wrong number of arguments\n");
    }

    if(!isscalar(prhs[1])){
        mexErrMsgTxt("Invalid Handle!\n");
    }

    if(!mxIsChar(prhs[2]))
        mexErrMsgTxt("Invalid path argument\n");

    if(!mxIsChar(prhs[3]))
        mexErrMsgTxt("Invalid attribute name.\n");

    if(!mxIsChar(prhs[4]))
        mexErrMsgTxt("Invalid text value.\n");


    handle = mxToInt(prhs[1]);
    mxToString(prhs[2],&xpath);
    mxToString(prhs[3],&name );
    mxToString(prhs[4],&val);
    handleTixiError(tixiAddTextAttribute(handle, xpath, name, val));
}

void mex_tixiAddDoubleAttribute(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    char * xpath = NULL;
    char * name  = NULL;
    char * format= NULL;
    double val   = 0.;
    int handle   = -1;

    if(nrhs != 6){
        mexErrMsgTxt("tixiAddDoubleAttribute(handle, path, attribName, value, formatstring): Wrong number of arguments\n");
    }

    if(!isscalar(prhs[1])){
        mexErrMsgTxt("Invalid Handle!\n");
    }

    if(!mxIsChar(prhs[2]))
        mexErrMsgTxt("Invalid path argument\n");

    if(!mxIsChar(prhs[3]))
        mexErrMsgTxt("Invalid attribute name.\n");

    if(!isscalar(prhs[4]))
        mexErrMsgTxt("Invalid value.\n");

    if(!mxIsChar(prhs[5]))
        mexErrMsgTxt("Invalid format string.\n");


    handle = mxToInt(prhs[1]);
    mxToString(prhs[2],&xpath);
    mxToString(prhs[3],&name );
    val = *mxGetPr(prhs[4]);
    mxToString(prhs[5],&format);
    handleTixiError(tixiAddDoubleAttribute(handle, xpath, name, val, format));
}

void mex_tixiAddIntegerAttribute(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    char * xpath = NULL;
    char * name  = NULL;
    char * format= NULL;
    int val   = 0;
    int handle   = -1;

    if(nrhs != 6){
        mexErrMsgTxt("tixiAddIntegerAttribute(handle, path, attribName, value, formatstring): Wrong number of arguments\n");
    }

    if(!isscalar(prhs[1])){
        mexErrMsgTxt("Invalid Handle!\n");
    }

    if(!mxIsChar(prhs[2]))
        mexErrMsgTxt("Invalid path argument\n");

    if(!mxIsChar(prhs[3]))
        mexErrMsgTxt("Invalid attribute name.\n");

    if(!isscalar(prhs[4]))
        mexErrMsgTxt("Invalid value.\n");

    if(!mxIsChar(prhs[5]))
        mexErrMsgTxt("Invalid format string.\n");


    handle = mxToInt(prhs[1]);
    mxToString(prhs[2],&xpath);
    mxToString(prhs[3],&name );
    val = (int) *mxGetPr(prhs[4]);
    mxToString(prhs[5],&format);
    handleTixiError(tixiAddIntegerAttribute(handle, xpath, name, val, format));
}

void mex_tixiGetTextAttribute(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    char * xpath = NULL;
    char * name  = NULL;
    char * val   = NULL;
    int handle   = -1;

    if(nrhs != 4){
        mexErrMsgTxt("tixiGetTextAttribute(handle, path, attribName): Wrong number of arguments\n");
    }

    if(!isscalar(prhs[1])){
        mexErrMsgTxt("Invalid Handle!\n");
    }

    if(!mxIsChar(prhs[2]))
        mexErrMsgTxt("Invalid path argument\n");

    if(!mxIsChar(prhs[3]))
        mexErrMsgTxt("Invalid attribute name.\n");


    handle = mxToInt(prhs[1]);
    mxToString(prhs[2],&xpath);
    mxToString(prhs[3],&name );
    handleTixiError(tixiGetTextAttribute(handle, xpath, name, &val));

    plhs[0] = mxCreateString(val);
}

void mex_tixiGetIntegerAttribute(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    char * xpath = NULL;
    char * name  = NULL;
    int val      = 0;
    int handle   = -1;

    if(nrhs != 4){
        mexErrMsgTxt("tixiGetIntegerAttribute(handle, path, attribName): Wrong number of arguments\n");
    }

    if(!isscalar(prhs[1])){
        mexErrMsgTxt("Invalid Handle!\n");
    }

    if(!mxIsChar(prhs[2]))
        mexErrMsgTxt("Invalid path argument\n");

    if(!mxIsChar(prhs[3]))
        mexErrMsgTxt("Invalid attribute name.\n");


    handle = mxToInt(prhs[1]);
    mxToString(prhs[2],&xpath);
    mxToString(prhs[3],&name );
    handleTixiError(tixiGetIntegerAttribute(handle, xpath, name, &val));

    plhs[0] = mxCreateDoubleMatrix(1,1,mxREAL);
    *mxGetPr(plhs[0]) = (double) val;
}

void mex_tixiGetBooleanAttribute(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    char * xpath = NULL;
    char * name  = NULL;
    int val      = 0;
    int handle   = -1;

    if(nrhs != 4){
        mexErrMsgTxt("tixiGetBooleanAttribute(handle, path, attribName): Wrong number of arguments\n");
    }

    if(!isscalar(prhs[1])){
        mexErrMsgTxt("Invalid Handle!\n");
    }

    if(!mxIsChar(prhs[2]))
        mexErrMsgTxt("Invalid path argument\n");

    if(!mxIsChar(prhs[3]))
        mexErrMsgTxt("Invalid attribute name.\n");


    handle = mxToInt(prhs[1]);
    mxToString(prhs[2],&xpath);
    mxToString(prhs[3],&name );
    handleTixiError(tixiGetBooleanAttribute(handle, xpath, name, &val));

    plhs[0] = mxCreateDoubleMatrix(1,1,mxREAL);
    *mxGetPr(plhs[0]) = (double) val;
}

void mex_tixiGetDoubleAttribute(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    char * xpath = NULL;
    char * name  = NULL;
    double val   = 0.;
    int handle   = -1;

    if(nrhs != 4){
        mexErrMsgTxt("tixiGetDoubleAttribute(handle, path, attribName): Wrong number of arguments\n");
    }

    if(!isscalar(prhs[1])){
        mexErrMsgTxt("Invalid Handle!\n");
    }

    if(!mxIsChar(prhs[2]))
        mexErrMsgTxt("Invalid path argument\n");

    if(!mxIsChar(prhs[3]))
        mexErrMsgTxt("Invalid attribute name.\n");


    handle = mxToInt(prhs[1]);
    mxToString(prhs[2],&xpath);
    mxToString(prhs[3],&name );
    handleTixiError(tixiGetDoubleAttribute(handle, xpath, name, &val));

    plhs[0] = mxCreateDoubleMatrix(1,1,mxREAL);
    *mxGetPr(plhs[0]) =  val;
}

/*
 We return true or false if element could be found/not found. If something else happened we
 throw an exception.
*/
void mex_tixiCheckElement(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    char * xpath = NULL;
    int handle   = -1;
    ReturnCode ret = SUCCESS;

    if(nrhs != 3){
        mexErrMsgTxt("tixiCheckElement(handle, path): Wrong number of arguments\n");
    }

    if(!isscalar(prhs[1])){
        mexErrMsgTxt("Invalid Handle!\n");
    }

    if(!mxIsChar(prhs[2]))
        mexErrMsgTxt("Invalid elementPath argument\n");

    handle = mxToInt(prhs[1]);
    mxToString(prhs[2],&xpath);

    ret = tixiCheckElement (handle, xpath);
    mxFree(xpath);

    if(ret == SUCCESS){
        plhs[0] = mxCreateLogicalScalar(1);
    }
    else if(ret == ELEMENT_NOT_FOUND){
        plhs[0] = mxCreateLogicalScalar(0);
    }
    else {
        handleTixiError(ret);
    }
}

/*
 We return true or false if attribute could be found/not found. If something else happened we
 throw an exception.
*/
void mex_tixiCheckAttribute(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    char * xpath = NULL;
    char * name  = NULL;
    int handle   = -1;
    ReturnCode ret = SUCCESS;

    if(nrhs != 4){
        mexErrMsgTxt("tixiCheckAttribute(handle, path, name): Wrong number of arguments\n");
    }

    if(!isscalar(prhs[1])){
        mexErrMsgTxt("Invalid Handle!\n");
    }

    if(!mxIsChar(prhs[2]))
        mexErrMsgTxt("Invalid path argument\n");

    if(!mxIsChar(prhs[3]))
        mexErrMsgTxt("Invalid name argument\n");

    handle = mxToInt(prhs[1]);
    mxToString(prhs[2],&xpath);
    mxToString(prhs[3],&name);

    ret = tixiCheckAttribute (handle, xpath, name);
    mxFree(xpath);
    mxFree(name);

    if(ret == SUCCESS){
        plhs[0] = mxCreateLogicalScalar(1);
    }
    else if(ret == ATTRIBUTE_NOT_FOUND){
        plhs[0] = mxCreateLogicalScalar(0);
    }
    else {
        handleTixiError(ret);
    }
}

/*
 We return true or false if handle is okay/ not okay. If something else happened we
 throw an exception.
*/
void mex_tixiCheckDocumentHandle(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    int handle   = -1;
    ReturnCode ret = SUCCESS;

    if(nrhs != 2){
        mexErrMsgTxt("tixiCheckDocumentHandle(handle): Wrong number of arguments\n");
    }

    if(!isscalar(prhs[1])){
        mexErrMsgTxt("Invalid Handle!\n");
    }

    handle = mxToInt(prhs[1]);

    ret = tixiCheckDocumentHandle(handle);

    if(ret == SUCCESS){
        plhs[0] = mxCreateLogicalScalar(1);
    }
    else if(ret == INVALID_HANDLE){
        plhs[0] = mxCreateLogicalScalar(0);
    }
    else {
        handleTixiError(ret);
    }
}

void mex_tixiAddExternalLink(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    char * xpath = NULL;
    char * url  = NULL;
    char * format= NULL;
    int handle   = -1;

    if(nrhs != 5){
        mexErrMsgTxt("tixiAddExternalLink(handle, path, url, fileFormat): Wrong number of arguments\n");
    }

    if(!isscalar(prhs[1])){
        mexErrMsgTxt("Invalid Handle!\n");
    }

    if(!mxIsChar(prhs[2]))
        mexErrMsgTxt("Invalid path argument\n");

    if(!mxIsChar(prhs[3]))
        mexErrMsgTxt("Invalid url.\n");

    if(!mxIsChar(prhs[4]))
        mexErrMsgTxt("Invalid file format.\n");

    handle = mxToInt(prhs[1]);
    mxToString(prhs[2],&xpath);
    mxToString(prhs[3],&url );
    mxToString(prhs[4],&format);
    handleTixiError(tixiAddExternalLink(handle, xpath, url, format));
    mxFree(url);
    mxFree(xpath);
    mxFree(format);
}


void mex_tixiUsePrettyPrint(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    int handle = -1;
    int prettyprint = 1;
    if(nrhs != 3){
        mexErrMsgTxt("tixiUsePrettyPrint(handle, boolean): Wrong number of arguments\n");
    }

    if(!isscalar(prhs[1])){
        mexErrMsgTxt("Invalid Handle!\n");
    }

    if(!mxIsLogicalScalar(prhs[2]))
        mexErrMsgTxt("Invalid prettyPrint argument\n");


    handle = mxToInt(prhs[1]);
    prettyprint = *mxGetLogicals(prhs[2]);
    handleTixiError(tixiUsePrettyPrint(handle, prettyprint));
}

void mex_tixiXSLTransformationToFile(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    char * xslfile = NULL;
    char * resultfile  = NULL;
    int handle   = -1;

    if(nrhs != 4){
        mexErrMsgTxt("tixiXSLTransformationToFile(handle, xslFilename, resultFileName): Wrong number of arguments\n");
    }

    if(!isscalar(prhs[1])){
        mexErrMsgTxt("Invalid Handle!\n");
    }

    if(!mxIsChar(prhs[2]))
        mexErrMsgTxt("Invalid xslFilename argument\n");

    if(!mxIsChar(prhs[3]))
        mexErrMsgTxt("Invalid resultFileName argument\n");

    handle = mxToInt(prhs[1]);
    mxToString(prhs[2],&xslfile);
    mxToString(prhs[3],&resultfile);

    handleTixiError(tixiCheckAttribute (handle, xslfile, resultfile));
    mxFree(xslfile);
    mxFree(resultfile);
}

void mex_tixiXPathEvaluateNodeNumber(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    char * xPathExpression = NULL;
    int number   = 0;
    int handle   = -1;

    if(nrhs != 3){
        mexErrMsgTxt("tixiXPathEvaluateNodeNumber(handle, xPathExpression): Wrong number of arguments\n");
    }

    if(!isscalar(prhs[1])){
        mexErrMsgTxt("Invalid Handle!\n");
    }

    if(!mxIsChar(prhs[2]))
        mexErrMsgTxt("Invalid xslFilename argument\n");

    handle = mxToInt(prhs[1]);
    mxToString(prhs[2],&xPathExpression);

    handleTixiError(tixiXPathEvaluateNodeNumber(handle, xPathExpression, &number));
    mxFree(xPathExpression);

    plhs[0] = mxCreateDoubleMatrix(1,1,mxREAL);
    *mxGetPr(plhs[0]) = (double) number;
}


void mex_tixiXPathExpressionGetTextByIndex(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    char * xPathExpression = NULL;
    char * text            = NULL; /* out */
    int number   = 0;
    int handle   = -1;

    if(nrhs != 4){
        mexErrMsgTxt("tixiXPathExpressionGetTextByIndex(handle, xPathExpression, elementNumber): Wrong number of arguments\n");
    }

    if(!isscalar(prhs[1])){
        mexErrMsgTxt("Invalid Handle!\n");
    }

    if(!mxIsChar(prhs[2]))
        mexErrMsgTxt("Invalid xslFilename argument\n");

    if(!isscalar(prhs[3]) || mxIsChar(prhs[3]))
        mexErrMsgTxt("Invalid number argument");

    handle = mxToInt(prhs[1]);
    mxToString(prhs[2],&xPathExpression);
    number = (int) *mxGetPr(prhs[3]);

    handleTixiError(tixiXPathExpressionGetTextByIndex(handle, xPathExpression, number, &text));
    mxFree(xPathExpression);

    plhs[0] = mxCreateString(text);
}


/*
 We return true or false if uid is not duplicated/duplicated. If something else happened we
 throw an exception.
*/
void mex_tixiUIDCheckDuplicates(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    int handle   = -1;
    ReturnCode ret = SUCCESS;

    if(nrhs != 2){
        mexErrMsgTxt("tixiUIDCheckDuplicates(handle): Wrong number of arguments\n");
    }

    if(!isscalar(prhs[1])){
        mexErrMsgTxt("Invalid Handle!\n");
    }

    handle = mxToInt(prhs[1]);

    ret = tixiUIDCheckDuplicates(handle);

    if(ret == SUCCESS){
        plhs[0] = mxCreateLogicalScalar(1);
    }
    else if(ret == UID_NOT_UNIQUE){
        plhs[0] = mxCreateLogicalScalar(0);
    }
    else {
        handleTixiError(ret);
    }
}

void mex_tixiUIDCheckLinks(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    int handle   = -1;

    if(nrhs != 2){
        mexErrMsgTxt("tixiUIDCheckLinks(handle): Wrong number of arguments\n");
    }

    if(!isscalar(prhs[1])){
        mexErrMsgTxt("Invalid Handle!\n");
    }

    handle = mxToInt(prhs[1]);

    handleTixiError(tixiUIDCheckDuplicates(handle));
}

/*
 We return true or false if uid is exists/not exists. If something else happened we
 throw an exception.
*/
void mex_tixiUIDCheckExists(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    int handle   = -1;
    char * uid    = NULL;
    ReturnCode ret = SUCCESS;

    if(nrhs != 3){
        mexErrMsgTxt("tixiUIDCheckExists(handle, uid): Wrong number of arguments\n");
    }

    if(!isscalar(prhs[1])){
        mexErrMsgTxt("Invalid Handle!\n");
    }

    if(!mxIsChar(prhs[2]))
        mexErrMsgTxt("Invalid uid\n");

    handle = mxToInt(prhs[1]);
    mxToString(prhs[2],&uid);

    ret = tixiUIDCheckExists(handle, uid);
    mxFree(uid);

    if(ret == SUCCESS){
        plhs[0] = mxCreateLogicalScalar(1);
    }
    else if(ret == UID_DONT_EXISTS){
        plhs[0] = mxCreateLogicalScalar(0);
    }
    else {
        handleTixiError(ret);
    }
}


/*
 We return true or false if uid is exists/not exists. If something else happened we
 throw an exception.
*/
void mex_tixiUIDGetXPath(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    int handle   = -1;
    char * uid    = NULL;
    char * xpath  = NULL;

    if(nrhs != 3){
        mexErrMsgTxt("tixiUIDGetXPath(handle, uid): Wrong number of arguments\n");
    }

    if(!isscalar(prhs[1])){
        mexErrMsgTxt("Invalid Handle!\n");
    }

    if(!mxIsChar(prhs[2]))
        mexErrMsgTxt("Invalid uid\n");

    handle = mxToInt(prhs[1]);
    mxToString(prhs[2],&uid);

    handleTixiError(tixiUIDGetXPath(handle, uid, &xpath));
    mxFree(uid);

    plhs[0] = mxCreateString(xpath);
}

/*
 We return true or false if uid is exists/not exists. If something else happened we
 throw an exception.
*/
void mex_tixiUIDSetToXPath(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    int handle   = -1;
    char * uid    = NULL;
    char * xpath  = NULL;

    if(nrhs != 4){
        mexErrMsgTxt("tixiUIDSetToXPath(handle, xpath, uid): Wrong number of arguments\n");
    }

    if(!isscalar(prhs[1])){
        mexErrMsgTxt("Invalid Handle!\n");
    }

    if(!mxIsChar(prhs[2]))
        mexErrMsgTxt("Invalid xpath\n");

    if(!mxIsChar(prhs[3]))
        mexErrMsgTxt("Invalid UID\n");


    handle = mxToInt(prhs[1]);
    mxToString(prhs[2],&xpath);
    mxToString(prhs[3],&uid);

    handleTixiError(tixiUIDSetToXPath(handle, xpath, uid));
    mxFree(uid);
    mxFree(xpath);
}

void mex_tixiGetChildElementName(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    char * xpath = NULL;
    char * name   = NULL;
    int index = -1;
    int handle   = -1;

    if(nrhs != 4){
        mexErrMsgTxt("tixiGetChildElementName(handle, path, index): Wrong number of arguments\n");
    }

    if(!isscalar(prhs[1])){
        mexErrMsgTxt("Invalid Handle!\n");
    }

    if(!mxIsChar(prhs[2]))
        mexErrMsgTxt("Invalid path argument\n");

    if(!isscalar(prhs[3]))
        mexErrMsgTxt("Invalid index.\n");


    handle = mxToInt(prhs[1]);
    mxToString(prhs[2],&xpath);
    index = mxToInt(prhs[3]);
    handleTixiError(tixiGetChildElementName(handle, xpath, index, &name));

    plhs[0] = mxCreateString(name);
    mxFree(xpath);
}

void mex_tixiGetAttributeName(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    char * xpath = NULL;
    char * name   = NULL;
    int index = -1;
    int handle   = -1;

    if(nrhs != 4){
        mexErrMsgTxt("tixiGetAttributeName(handle, path, index): Wrong number of arguments\n");
    }

    if(!isscalar(prhs[1])){
        mexErrMsgTxt("Invalid Handle!\n");
    }

    if(!mxIsChar(prhs[2]))
        mexErrMsgTxt("Invalid path argument\n");

    if(!isscalar(prhs[3]))
        mexErrMsgTxt("Invalid index.\n");


    handle = mxToInt(prhs[1]);
    mxToString(prhs[2],&xpath);
    index = mxToInt(prhs[3]);
    handleTixiError(tixiGetAttributeName(handle, xpath, index, &name));

    plhs[0] = mxCreateString(name);
    mxFree(xpath);
}

void mex_tixiGetNumberOfChilds(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    char * xpath = NULL;
    int handle   = -1;
    int nChilds = 0;

    if(nrhs != 3){
        mexErrMsgTxt("tixiGetNumberOfChilds(handle, path): Wrong number of arguments\n");
    }

    if(!isscalar(prhs[1])){
        mexErrMsgTxt("Invalid Handle!\n");
    }

    if(!mxIsChar(prhs[2]))
        mexErrMsgTxt("Invalid path argument\n");


    handle = mxToInt(prhs[1]);
    mxToString(prhs[2],&xpath);

    handleTixiError(tixiGetNumberOfChilds(handle, xpath, &nChilds));

    plhs[0] = mxCreateDoubleMatrix(1,1,mxREAL);
    *mxGetPr(plhs[0]) = (double) nChilds;
    
    mxFree(xpath);
}

void mex_tixiGetNumberOfAttributes(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    char * xpath = NULL;
    int handle   = -1;
    int nAttribs = 0;

    if(nrhs != 3){
        mexErrMsgTxt("tixiGetNumberOfAttributes(handle, path): Wrong number of arguments\n");
    }

    if(!isscalar(prhs[1])){
        mexErrMsgTxt("Invalid Handle!\n");
    }

    if(!mxIsChar(prhs[2]))
        mexErrMsgTxt("Invalid path argument\n");


    handle = mxToInt(prhs[1]);
    mxToString(prhs[2],&xpath);

    handleTixiError(tixiGetNumberOfAttributes(handle, xpath, &nAttribs));

    plhs[0] = mxCreateDoubleMatrix(1,1,mxREAL);
    *mxGetPr(plhs[0]) = (double) nAttribs;
    
    mxFree(xpath);
}

void mex_closeTixi(void){
    mexPrintf("Closing tixi...\n");
    tixiCleanup();
}

/*
 * main entry point for MATLAB
 * deals as a dispatcher here
 */
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  char * functionName = NULL;
  if(nrhs < 1){
      mexErrMsgTxt("No function argument given!");
      return;
  }
  
  mexAtExit(mex_closeTixi);
  mxToString(prhs[0],&functionName);

  /* just a macro that saves typing */
#define RUN_IF_FUNCTION_IS(fname)           \
  if(strcmp(functionName, #fname) == 0) {   \
      mex_ ## fname(nlhs, plhs, nrhs, prhs);\
      mxFree(functionName);                 \
      return;                               \
  }

  RUN_IF_FUNCTION_IS(tixiOpenDocument)
  RUN_IF_FUNCTION_IS(tixiOpenDocumentRecursive)
  RUN_IF_FUNCTION_IS(tixiOpenDocumentFromHTTP)
  RUN_IF_FUNCTION_IS(tixiCreateDocument)
  RUN_IF_FUNCTION_IS(tixiCloseDocument)
  RUN_IF_FUNCTION_IS(tixiCloseAllDocuments)
  RUN_IF_FUNCTION_IS(tixiSchemaValidateFromFile)
  RUN_IF_FUNCTION_IS(tixiSchemaValidateFromString)
  RUN_IF_FUNCTION_IS(tixiDTDValidate)
  RUN_IF_FUNCTION_IS(tixiGetTextElement)
  RUN_IF_FUNCTION_IS(tixiGetDoubleElement)
  RUN_IF_FUNCTION_IS(tixiGetIntegerElement)
  RUN_IF_FUNCTION_IS(tixiGetBooleanElement)
  RUN_IF_FUNCTION_IS(tixiUpdateTextElement)
  RUN_IF_FUNCTION_IS(tixiUpdateDoubleElement)
  RUN_IF_FUNCTION_IS(tixiAddTextElement)
  RUN_IF_FUNCTION_IS(tixiAddBooleanElement)
  RUN_IF_FUNCTION_IS(tixiAddDoubleElement)
  RUN_IF_FUNCTION_IS(tixiAddIntegerElement)
  RUN_IF_FUNCTION_IS(tixiAddTextElementAtIndex)
  RUN_IF_FUNCTION_IS(tixiRemoveElement)
  RUN_IF_FUNCTION_IS(tixiRemoveAttribute)
  RUN_IF_FUNCTION_IS(tixiSaveDocument)
  RUN_IF_FUNCTION_IS(tixiSaveCompleteDocument)
  RUN_IF_FUNCTION_IS(tixiSaveAndRemoveDocument)
  RUN_IF_FUNCTION_IS(tixiExportDocumentAsString)
  RUN_IF_FUNCTION_IS(tixiImportFromString)
  RUN_IF_FUNCTION_IS(tixiGetVectorSize)
  RUN_IF_FUNCTION_IS(tixiGetFloatVector)
  RUN_IF_FUNCTION_IS(tixiAddFloatVector)
  RUN_IF_FUNCTION_IS(tixiCreateElement)
  RUN_IF_FUNCTION_IS(tixiCreateElementAtIndex)
  RUN_IF_FUNCTION_IS(tixiAddHeader)
  RUN_IF_FUNCTION_IS(tixiAddCpacsHeader)
  RUN_IF_FUNCTION_IS(tixiGetNamedChildrenCount)
  RUN_IF_FUNCTION_IS(tixiAddPoint)
  RUN_IF_FUNCTION_IS(tixiGetPoint)
  RUN_IF_FUNCTION_IS(tixiAddTextAttribute)
  RUN_IF_FUNCTION_IS(tixiAddIntegerAttribute)
  RUN_IF_FUNCTION_IS(tixiAddDoubleAttribute)
  RUN_IF_FUNCTION_IS(tixiGetTextAttribute)
  RUN_IF_FUNCTION_IS(tixiGetIntegerAttribute)
  RUN_IF_FUNCTION_IS(tixiGetBooleanAttribute)
  RUN_IF_FUNCTION_IS(tixiGetDoubleAttribute)
  RUN_IF_FUNCTION_IS(tixiGetVersion)
  RUN_IF_FUNCTION_IS(tixiCheckElement)
  RUN_IF_FUNCTION_IS(tixiCheckAttribute)
  RUN_IF_FUNCTION_IS(tixiCheckDocumentHandle)
  RUN_IF_FUNCTION_IS(tixiAddExternalLink)
  RUN_IF_FUNCTION_IS(tixiUsePrettyPrint)
  RUN_IF_FUNCTION_IS(tixiXSLTransformationToFile)
  RUN_IF_FUNCTION_IS(tixiXPathEvaluateNodeNumber)
  RUN_IF_FUNCTION_IS(tixiXPathExpressionGetTextByIndex)
  RUN_IF_FUNCTION_IS(tixiUIDCheckDuplicates)
  RUN_IF_FUNCTION_IS(tixiUIDCheckLinks)
  RUN_IF_FUNCTION_IS(tixiUIDCheckExists)
  RUN_IF_FUNCTION_IS(tixiUIDGetXPath)
  RUN_IF_FUNCTION_IS(tixiUIDSetToXPath)
  RUN_IF_FUNCTION_IS(tixiGetChildElementName)
  RUN_IF_FUNCTION_IS(tixiGetNumberOfChilds)
  RUN_IF_FUNCTION_IS(tixiGetAttributeName)
  RUN_IF_FUNCTION_IS(tixiGetNumberOfAttributes)
  {
      // this is only executed if the function could not be identified
      char text[255];
      snprintf(text, 250, "%s is not a valid TIXI function!\n",functionName);
      mxFree(functionName);
      mexErrMsgTxt(text);
  }
}

#ifdef __cplusplus
}
#endif
