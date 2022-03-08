# TIXI #

[![CI](https://github.com/DLR-SC/tixi/actions/workflows/main.yml/badge.svg)](https://github.com/DLR-SC/tixi/actions/workflows/main.yml)

 - Binary Downloads:  https://github.com/DLR-SC/tixi/wiki/Downloads
 - API Documentation: http://dlr-sc.github.io/tixi/
 - Issue Tracker:     https://github.com/DLR-SC/tixi/issues
 - Wiki:              https://github.com/DLR-SC/tixi/wiki

## Installation ##
 - with Conda: [![Anaconda-Server Badge](https://anaconda.org/dlr-sc/tixi3/badges/installer/conda.svg)](https://anaconda.org/DLR-SC/tixi3)
 - with Linux package manager: [OpenBuildService](https://software.opensuse.org/download.html?project=science:dlr&package=tixi3)


## Description ##
TiXI is a fast and simple XML interface library and could be used from applications written in C, C++, Fortran, JAVA and Python. The library can be directly integrated into a code by third party software or can be used by users who don't want to deal with the complexity of XML when creating a new application. Although simplified and somewhat restricted compared to a full-fledged XML processing library the user can, for example, create documents, create and delete nodes, and add and remove element attributes. Routines to read and write simple text nodes and additionally specialized nodes holding integer and floating point numbers are part of this API. Furthermore, routines to process aggregates of these simple types have been implemented. For the processing of geometric data, reading and writing of multidimensional arrays or arrays of vectors, i.e. coordinates of points are supported. The library has been designed to hide the implementation details so that the underlying XML library, currently libxml2, can be replaced by another one without changing the XML processing API in the applications.

Reading a text attribute could be as easy as:

```
tixiGetTextAttribute( handle, elementPath, attributeName, &attributeValue );
```

Getting a double value would look like this:
```
tixiGetDoubleElement( handle, elementPath, &x );
```

## Multi Language Support ##
The TIXI library is written in C, but there are interfaces and wrappers for C++, Fortran, Python, JAVA and Matlab. Take a look at our examples for [C](https://github.com/DLR-SC/tixi/wiki/CExamples) and [Fortran](https://github.com/DLR-SC/tixi/wiki/Fortran%20Examples).
