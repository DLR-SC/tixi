Changelog
=========

Version 3.3.0
-------------

API Changes:

 - ``::tixiXPathEvaluateNodeNumber`` will not throw an error anymore when an xml node does
   not exist. Rather, the function will succeed and return zero. (issue #198)
 - Fixed inconsistent behavior of the function ``::tixiUpdateTextElement``: This function should 
   only work, when the element path refers to a single element with no XML child nodes or at most 
   one text child node. This is now explicitly checked and an error is thrown, if these
   prerequisites are not met. Without this, it is not clear how the function behaves e.g. at the 
   presence of comment child nodes.
 - ``::tixiXPathExpressionGetTextByIndex`` now retrieves the complete content of an element
   including text and comments.

New Functions:

 - ``::tixiExportElementAsString`` retrieves the xml string of a given element (issue #202)
 - ``::tixiImportElementFromString`` imports a char-string as a child element into an existing 
   tixi-document (issue #202).

Version 3.2.0
-------------

API Changes:

 - Fixed inconsistent behaviour of the function ``::tixiAddExternalLink``. 
   The function now creates external node links consistent to ``::tixiOpenDocumentRecursive``.
   A new required argument defines, whether the newly created external node
   should be directly loaded into the xml tree (issue #173).

General Changes:

 - Drasting performance improvements when adding and changing vectors via 
   ``::tixiAddFloatVector`` and ``::tixiUpdateFloatVector`` by changing
   the quadratic runtime behaviour to a linear behaviour (issue #183).
 - Added support for Python 3.8 and Python 3.9 (issue #178).


Fixes:
- Fixed invalid calls in matlab bindings (issue #170).
- Fixed error code in ``::tixiGetFloatVector``, if a vector element is not a number (issue #184).
- Fixed a crash when saving to external xml documents (issue #175). 


Version 3.1.1
-------------

Fixes:
- Fix for `::tixiGetDoubleElement` and `::tixiGetIntegerElement` and elements with trailing whitespace.
- Removed all exit( ) calls in TiXI

Version 3.1.0
----------------

API Changes:
- Deprecated all array functions. These are

  - `::tixiGetArray`
  - `::tixiGetArrayDimensionNames`
  - `::tixiGetArrayDimensions`
  - `::tixiGetArrayDimensionSizes`
  - `::tixiGetArrayDimensionValues`
  - `::tixiGetArrayElementCount`
  - `::tixiGetArrayElementNames`
  - `::tixiGetArrayParameterNames`
  - `::tixiGetArrayParameters`
  - `::tixiGetArrayValue`

  **These functions will be removed in TiXI 3.3.0!**

- ```::tixiGetDoubleElement``` and ```::tixiGetIntegerElement``` now return the error code ```::NO_NUMBER```,
	  if the element is not a number.
	  Before, TiXI returned '0' as a number without error.
- ```::tixiGetFloatVector``` and ```::tixiGetVectorSize``` do not check for `mapType="vector"` anymore.

General Changes:
- The validation error messages produced by
	```::tixiSchemaValidateFromFile```, ```::tixiSchemaValidateWithDefaultsFromFile``` and
	```::tixiSchemaValidateFromString``` now show line numbers of the error in the xml file.
- Support of Matlab R2018a and newer

New Functions:
- Added new function ```::tixiRemoveExternalLinks``` to remove links to external files to
  improve validation of xml files.

Fixes:
- Fixed several build problems
- Fixed adding empty vectors using ```::tixiAddFloatVector```
- Fixed crash for very long error messages
- Fixed decoding of special characters by ```::tixiGetTextElement```
	
Version 3.0.3
----------------

General Changes:
- Improved cmake export scripts. Tixi installs should now be relocatable.
- The cmake build system now builds by default static libraries. If shared
  libraries are desired, ```-DBUILD_SHARED_LIBS=ON``` has to be passed to cmake.
- Implemented caching for the functions ```tixiXPathExpressionGetXPath``` and
	```tixiXPathEvaluateNodeNumber``` to speed up queries on large files.

New Functions:
- ```tixiSwapElements``` allows to exchange two xml elements.
- Added functions TixiGetNumberOfChilds, TixiGetChildNodeName and
  TixiGetChildElementPaths to C++ Bindings.

Fixes:
- Fixed indexing of text nodes. Now it is possible to directly select text nodes
  using e.g. /root/a/text()[2].
- Fixed function ```tixiGetChildNodeName``` to returns also the namespace prefix.

Version 3.0.2
----------------

New Functions:
- tixiRenameElement To rename an existing element
	

Version 3.0.1
----------------

General Changes:
- Completed C++ wrapper

New Functions:
- tixiXSLTransformationToString To perform an xml transformation
	
Fixes:
- Fixed an error in Python bindings that added a tixi prefix in front of all methods

Version 3.0
---------------

General Changes:
- Renamed TIXI library to tixi3
- TiXI header file is now located under include/tixi3/tixi.h
- Added new C++ bindings for TiXI located under include/tixi3/tixicpp.h

	
Fixes:
- Fixed failure in ```tixiUIDCheckExists``` if a uID is duplicate
- Fixed invalid characters in Matlab bindings
- Fixed missing return value in the matlab bindings of ```tixiUIDCheckExists```
- Fixed compilation of the matlab bindings on linux

Version 2.2.4
---------------

General Changes:
	- Added XML namespace support
	- Added Makefile to build the MATLAB interface with mex

New Functions:
- Functions to write elements with namespaces

	- tixiCreateElementNS
	- tixiCreateElementNSAtIndex
	- tixiAddBooleanElementNS
	- tixiAddDoubleElementNS
	- tixiAddIntegerElementNS
	- tixiAddTextElementNS
	- tixiAddTextElementNSAtIndex

- Funtions for namespace modifications

	- tixiSetElementNamespace
	- tixiDeclareNamespace

- Functions to register namespaces

	- tixiRegisterNamespace
	- tixiRegisterNamespacesFromDocument

Fixes:
- Fixed hard-coded libraries in CMake config scripts


Version 2.2.3
---------------

General Changes:
- Removed warning in tixiOpenDocumentRecursive, if no external
  files are defined in the cpacs file
- Fixed problems with intel ifort in linux
- Added Fortran 03 interface. This has to be compiled by the user.
- The C-code of the matlab mex interface is now always shipped with tixi,
  if the user wants to build it by himself.

Version 2.2.2
---------------

General Changes:
- Less messages in case of errors to reduce clutter

Fixes:
- Fixed tixiUpdateDoubleElement and tixiUpdateIntegerElement not updating
  the element content, if the element was empty before


Version 2.2.1
---------------

API Changes:
- Removed ellipsis argument for error message handler. This
  change affects code calling tixiSetPrintMsgFunc.

New Functions:
- tixiGetPrintMsgFunc to return the internal message handler.


Version 2.2.0
---------------

General Changes:
- Support to load and save external data from directories
  relative to the XML file rather than the current working directory
- Completely cleaned up code base for a common code style 


New Functions:
- tixiXPathExpressionGetXPath to query an absolute XPath from
  an XPath search query
- tixiGetDocumentPath to get the path of the currently opened file

Fixes:
- All known memory issues fixed (more than 100)
- Fixed problems when dealing with external data
- Fixed many compiler warnings

	
Version 2.1.1
---------------

General Changes:
- Rerouting mechanism for error and warning messages 
  (a custom message handler can be installed)
- Added Python 3 support


API Changes:
- tixiAddFloatVector now has an additional precision parameter to control
  e.g. the number of digits after the decimal point

New Functions:
- tixiUpdateFloatVector to change the data of an already existing vector
- tixiSetPrintMsgFunc to install a custom message handler

Fixes:
- tixiGetNumberOfChilds and tixiGetChildNodeName now ignore DTD nodes
- MATLAB specific: reverted tixiCheckElement, tixiCheckAttribute, 
  tixiCheckDocumentHandle, tixiCheckUIDExists to old behavior


Version 2.1.0
---------------

General Changes:
- Automatic generation of python and matlab bindings
- TiXI can now be used as an Android NDK native module
- TiXI installs cmake config files in order to be used by other projects


API Changes:
- Renamed tixiGetChildElementName to tixiGetChildNodeName. Its output is now
  consistent with DOM standard.
- Removed tixiGetRawInterface
- Changed arguments in tixiAddPoint MATLAB binding
- Removed all matrix related functions

New Functions:
- tixiGetNodeType, returns the type of an XML node accoring to DOM standard.

	
Fixes:
- Fixed incorrect return value of tixiGetFloatVector, if number of elements
  read is smaller than expected
- TiXI can now be built with clang also on windows
- Fixed wrong formatting of the result XML, when inserting an xml element
  using tixiCreateElementAtIndex


Version 2.0.4
---------------

New Functions:
- tixiGetChildElementName - Returns the name of a child element beneath a given path.
- tixiGetAttributeName - Returns the name of an attribute of a given node.
- tixiGetNumberOfChilds - Returns the number of childs elements of a given path.
- tixiGetNumberOfAttributes - Returns the number of attributes of a given node.
- tixiGetBooleanAttribute
	
-Changes:
- Switched the API to const char strings. This should produce much less
  warnings but should not break the current API/ABI.
- Added file tixi_version.h including all version information.
- Added support for visual leak detector
- Added check for python version < 3.0 and python version > 2.5
	
Fixes:
- Fixed tixiUpdateTextElement not updating the text, if the node was empty before
- Fixed element retrieving of root node
- Fixed uIDCheckExists behaviour of python wrapper
	
	
Version 2.0.3
---------------

- Changed UnitTesting Framework to google-test
- Added coverage with gcov (gcc only) to project
- Added check that the number of elements in an array is the same as the specified array size
- Fixed null-pointer bug in tixiGetTextAttribute
- tixiAddCpacsHeader now creates also a CPACS version tag
- Documentation overhaul

New Functions:
- tixiUpdateIntegerElement
- tixiUpdateBooleanElement
	
	
Version 2.0.2
---------------

- Fixed lots of memory leaks
- Improved usability of uIDCheckExists (python)
- Changed unit testing framework to google test (gtest-1.6.0)
- Added more unit tests


Version 2.0.1
---------------

- Installer for 64-bit Windows
- All-In-One installer with 3rd party dependencies for Windows
- Added MATLAB interface


Version 2.0
---------------

- Changes to fit CPACS-2.0 and TIGL-2.0.


Version 1.1
---------------

- Using a more recent version of MSVC (2008)
- Adding a JAVA (OSGi) Bundle that allows easy usage of TIXI functions from java code to repository. Right now the most important functions are implemented

- New functions:
	- tixiUpdateDoubleElement replaces an double element of a node
	- tixiAddTextElementAtIndex could create a new element at a given position
	
	
Version 1.0
---------------

- Many functions now print more informative error messages
- Added a Python wrapper for the C-code and DLL handling (no need to manually convert cpython variables to python)

New Functions:
- tixiUIDCheckLinks (new function) performs a check over all nodes with the uID "isLink"
  and checks if the corresponding uid exists in that data set

Fixes:
- tixiRemoveAttribute does not remove an attribute
- tixiCheckAttribute does no longer write an error message when not finding attributes


Version 0.9
---------------

- New function tixiUsePrettyPrint to make pretty print optional
- New UID helper functions:
	- tixiUIDCheckDuplicates checks if all uID are unique
	- tixiUIDGetXPath gives the XPath to a given uID
	- tixiUIDCheckExists checks if a uID exists
	- tixiUIDSetToXPath sets a uID to a given node
	- tixiXPathEvaluateNodeNumber Evaluates a XPath expression and returns the number
	  of result nodes matching this xpath expression.
	- tixiXPathExpressionGetTextByIndex Evaluates a XPath expression and returns
	  the text content of the resultnode matching this xpath expression.
	
- Bugfix in tixiAddTextAttributes. The function does now replace a attribute
  if there is already one with the same name.
- Bugfix in tixiRemoveAttribute. The function does deletes a attribute from a node correctly.
- TIXI 64-Bit libs are available for linux.
