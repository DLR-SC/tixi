# Python wrapper code generator howto #

## For users ##
TIXI comes with a code generator that creates Python wrapper code for the TIXI library.
```
python make-wrapper.py
```
The generator takes the `tixi.h` header file and generates the `tixiwrapper.py` Python module useable in your own Python scripts.

The generated python file contains a `Tixi` class that wraps up all methods of the library in a convenient Pythonic way, a `TixiException` class and all enums defined in the C header file.

Each method has the same name as the C function with the `tixi` prefix removed and a lower-case first letter.
Some methods have one or more additional parameters that determine the return array size; this is obvious from the generated code, which should always serve as a reference.

As an example here is some simple code using the wrapper:

```
from tixiwrapper import *

t = Tixi()
try:
    t.open('my-cpacs-document.xml')
    t.schemaValidateFromFile('my-cpacs-schema.xsd')
except TixiException, e:
    print e.error
    raise e
version = t.getTextElement('/cpacs/header/version')
t.close()
print version
```

## For developers ##
By default, the wrapper generator checks if the first parameter is a `TixiHandle` and acts accordingly.
Additionally the last parameter in the function signature, if any, is assumed to be an output parameter. To suppress output value wrapping for functions that don't return anything, provide the following documentation pragma within the doxygen header:
```
#PY:#
```
To tell the wrapper generator about special method invocation cases, there were introduced some annotations within the doxygen-comments of the `tixi.h` interface to give some hints, for example in the function `tixiGetFloatMatrix`:
```
#PY:6:3;4#
```
Here is the system of annotations:

Within the matching doxygen documentation block of a method/function, put a string starting with `#PY:` and ending with another `#`.

Between these markers, put either:

  * the empty string to explicitly tell the generator to consider all function arguments as input parameters (otherwise, by default, the last argument is taken as an output parameter, if it is a pointer (has `*`))
  * a comma-separated list of natural numbers, where each means an input variable between zero and n, from left to right, excluding the handle parameter. Each listed number (slot) means one parameter of the `tixi.h` interface that is considered an output argument, so that the Python code returns a value for it
  * the above, plus `:` and a comma separated list of semicolon-separated natural numbers between zero and n, from left to right. Each inner list means a product of values from the input variables slots. There can at most be as many inner lists as there are in the previous comma-separated lists of output variables.

Allowed values for the inner lists are as follow:
  * `0` means "no array", the wrapper returns a simple Python value instead of an array
  * a positiv number means an input variable slot, whose number is multiplied to determine an output array size
  * a negative number means a user-specified additional input parameter that determines the output array size. Same indexes mean same slots

Here is an example:

```
#PY:2,4:1;2,-1#
```
represents two manually marked output variables as C-arguments 2 and 4 (0-based counting but disregarding the TIXI-handle slot), if present.
The first output parameter has as an inner list of 1 and 2 which means calculating the product of the values of input arguments in position 1 and 2.
The second output parameter has an inner list of -1 which means that the generator will add an additional input argument for manual input of the output array's size, because the wrapped method needs it and the wrapper generator cannot possibly know it in advance.