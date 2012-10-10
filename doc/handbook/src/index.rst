.. NeGeV: Next Generation Solver

.. :title-reference:`TIXI Handbook: Developer Manual and Reference Documentation`

################################################################
TIXI: XML Interface Library
################################################################

TIXI is a fast and simple XML interface library and could be used from applications written in C, C++, Fortran, JAVA and Python. 
The simplified XML processing library can be directly integrated into a code by third party software or can be used to create 
new application from people who dosn't want to deal with the complexity of XML. Although simplified and somewhat restricted 
compared to a full- edged XML processing library the user can, for example, create documents, create and delete nodes, and 
add and remove element attributes. Routines to read and write simple text nodes and additionally specialized nodes holding 
integer and floating point numbers are part of this API. Furthermore, routines to process aggregates of these simple types have 
been implemented. For the processing of geometric data, reading and writing of multidimensional arrays or arrays of vectors, i.e. 
coordinates of points are supported. The C API and the API for Fortran provide the same functionally to the programmer. The 
library has been designed to hide the implementation details so that the underlying XML library, currently libxml2, can be 
replaced by another one without changing the XML processing API in the applications.


TIXI home (google code):
https://code.google.com/p/tixi/
        
.. toctree::
   :maxdepth: 1
   :glob:
   
   contents
   frontmatter

This manual built |today|