# TIXI #

 - Binary Downloads:  https://github.com/DLR-SC/tixi/wiki/Downloads
 - API Documentation: http://tixi.sourceforge.net/Doc/index.html
 - Issue Tracker:     https://github.com/DLR-SC/tixi/issues
 - Wiki:              https://github.com/DLR-SC/tixi/wiki
 
## Description ##
TIXI is a fast and simple XML interface library and could be used from applications written in C, C++, Fortran, JAVA and Python.
The simplifed XML processing library can be directly integrated into a code by third party software or can be used to create a new application from people who dosn't want to deal with the complexity of XML. Although simplifed and somewhat restricted compared to a full-
edged XML processing library the user can, for example, create documents, create and delete nodes, and add and remove element attributes. Routines to read and write simple text nodes and additionally specialized nodes holding integer and floating point numbers are part of this API. Furthermore, routines to process aggregates of these simple types have been implemented. For the processing of geometric data, reading and writing of multidimensional arrays or arrays of vectors, i.e. coordinates of points are supported. The C API and the API for Fortran provide the same functionally to the programmer. The library has been designed to hide the implementation details so that the underlying XML library, currently libxml2, can be replaced by another one without changing the XML processing API in the applications.

Reading a text attribute could be as easy as:

```
tixiGetTextAttribute( handle, elementPath, attributeName, &attributeValue );
```

Getting a double value would look like this:
```
tixiGetDoubleElement( handle, elementPath, &x );
```

## Multi Language Support ##
The TIXI library is written in C, but there are interfaces and wrappers for C++, Fortran, Python, JAVA and Matlab. Take a look at our examples for [Python](https://github.com/DLR-SC/tixi/wiki/PythonExamples), [C](https://github.com/DLR-SC/tixi/wiki/CExamples) and [Fortran](https://github.com/DLR-SC/tixi/wiki/Fortran Examples).
