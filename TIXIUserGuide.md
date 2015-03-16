# Usage examples #

## Retrieve the position of the first wing ##
```
 #include "tixi.h"

 char* xmlFilename = "ju.xml";
 TixiDocumentHandle handle = -1;
 char * elementPath = "/plane/wings/wing[1]";
 char* attributeName = "position";

 char* attributeValue;

 tixiOpenDocument( xmlFilename, &handle );
 tixiGetTextAttribute( handle, elementPath, attributeName, &attributeValue );
 tixiCloseDocument( handle );
```


## Retrieve x value of the coordinate origin ##
```
 #include "tixi.h"

 char* xmlFilename = "ju.xml";
 TixiDocumentHandle handle = -1;
 char * elementPath = "/plane/coordinateOrigin/x";
 double x = 0.;

 tixiOpenDocument( xmlFilename, &handle );
 tixiGetDoubleElement( handle, elementPath, &x );
 tixiCloseDocument( handle );
```



# Notes for Fortran programmers #
The Fortran interface is implemented by calls to subroutines. It assumes the following mapping for the basic types: real is _real\*8_ corresponds _double_, _integer_ is _integer\*4_ corresponds to _int_, character corresponds char Character strings are to be passed as variables of type _character\*N_. If a string is returned by a subroutine call the variable holding the result must be large enough to hold the result. Otherwise the result is truncated and the return code is set to STRING TRUNCATED.

NOTE: In view of these restrictions an implementation using character
arrays should be considered. The return codes returned in the last  argument corresponds to their position in ReturnCode starting with 0. A routine will be supplied to directly access the meaning of a return code as a string. When the C interface requires to pass a NULL-pointer, e.g. to choose the default format _”%g”_ when writing floating point elements, the respective argument in the Fortran interfaces is the empty string constant _””_. This is the only way to represent a string of length zero. Passing a variable with all characters set to _””_ will, via the interface transformed into an emtpy C-string _”0”_, which is of length 1, and not to a NULL-pointer.

# Notes for Python programmers #
Please see PythonWrapper for more information.