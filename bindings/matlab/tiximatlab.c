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



//MEX_EXPORT void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]);

#ifdef _WIN32
#define snprintf _snprintf
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
  strLen = (mxGetM(mxStr) * mxGetN(mxStr)) + 1;

  /* Allocate memory for input and output strings. */
  *cstr = (char*) mxCalloc(strLen, sizeof(char));

  /* Copy the string data from prhs[0] into a C string 
   * input_buf. */
  return mxGetString(mxStr, *cstr, strLen);
}


int mxToInt(const mxArray * mxArr){
    double dv = *mxGetPr(mxArr);
    if(!isinteger(dv))
        mexWarnMsgTxt("Argument is not an integer\n");

    return (int)dv;
}

void mex_tixiOpenDocument(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    char * filename = NULL;
    TixiDocumentHandle handle;

    if(nrhs != 2){
        mexErrMsgTxt("tixiOpenDocument(filename): Wrong number of arguments\n");
    }

    // get the filename
    mxToString(prhs[1],&filename);
    if(tixiOpenDocument(filename, &handle)!=SUCCESS){
        mexErrMsgTxt("File can not be opened\n");
    }

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

    if(mxGetM(mxHandle)!= 1 || mxGetN(mxHandle) != 1){
        mexErrMsgTxt("Wrong handle format\n");
    }

    // get handle
    handle = (int) *mxGetPr(mxHandle);
    if(tixiCloseDocument(handle)!=SUCCESS)
        mexErrMsgTxt("Error closing document\n");
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
        mexErrMsgTxt("Ivalid xpath argument");


    handle = mxToInt(prhs[1]);
    mxToString(prhs[2],&xpath);
    if(tixiGetTextElement(handle, xpath, &text)!=SUCCESS)
        mexErrMsgTxt("Error retrieving text element\n");

    plhs[0] = mxCreateString(text);
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
        mexErrMsgTxt("Ivalid filename argument");


    handle = mxToInt(prhs[1]);
    mxToString(prhs[2],&filename);
    if(tixiSaveDocument(handle, filename)!=SUCCESS)
        mexErrMsgTxt("Error saving document\n");
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int status = 0;
    char * functionName = NULL;
    if(nrhs < 1){
        mexErrMsgTxt("No function argument given!");
        return;
    }
    
  mxToString(prhs[0],&functionName);

  if(strcmp(functionName,"tixiOpenDocument")==0){
      mex_tixiOpenDocument(nlhs, plhs, nrhs, prhs);
      return;
  }
  else if(strcmp(functionName,"tixiCloseDocument")==0){
      mex_tixiCloseDocument(nlhs, plhs, nrhs, prhs);
      return;
  }
  else if(strcmp(functionName,"tixiGetTextElement")==0){
      mex_tixiGetTextElement(nlhs, plhs, nrhs, prhs);
      return;
  }
  else if(strcmp(functionName,"tixiSaveDocument")==0){
      mex_tixiSaveDocument(nlhs, plhs, nrhs, prhs);
      return;
  }
  else {
      char text[255];
      snprintf(text, 250, "%s is not a valid TIXI function!\n",functionName);
      mexErrMsgTxt(text);
  }



}

#ifdef __cplusplus
}
#endif