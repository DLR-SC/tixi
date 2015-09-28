#!/usr/bin/python
#
# @file
# @brief Demo Python script to show how TIXI opens a xml file and includes all 
#             XML files that are linked in.
# @author Markus Litz, DLR/SC, 2008-06-23
#

from ctypes import *

# define handles
tixiHandle = c_int(0)
xmlInputFilename    = "MainFile.xml"
xmlOutputFilename   = "Result_out.xml"

# open TIXI and TIGL shared libraries
import sys
if sys.platform == 'win32':
    TIXI = cdll.TIXI
else:
    TIXI = CDLL("libTIXI.so")

# Open a CPACS configuration file. First open the CPACS-XML file
# with TIXI to get a tixi handle.
tixiReturn = TIXI.tixiOpenDocumentRecursive(xmlInputFilename, byref(tixiHandle), 1)
if tixiReturn != 0:
    print "Error: tixiOpenDocument failed for file: " + xmlInputFilename
    exit(1)
else:
	print "TIXI opened a xml file successfully!"

	
# remove an Element from the main xml file:
TIXI.tixiRemoveElement(tixiHandle, "/plane/aPoint")

# add an element to a node that is located in a external satellite file
TIXI.tixiAddTextElement(tixiHandle, "/plane/testNode/aVeryTest/testNode/aVeryTest3/point", "y", "123.45")


print
# export document as string
xmlDocumentString = c_char_p()
TIXI.tixiExportDocumentAsString(tixiHandle, byref(xmlDocumentString))
print "---------------------------------"
print "Plane Name: " + xmlDocumentString.value
print "---------------------------------"

#  After all elements have been edited, save the document to file with all changes in the sub-files.
error = TIXI.tixiSaveDocument( tixiHandle, xmlOutputFilename )
if error == 0:
	print "Document is written."
 
#  Now we can close the TIXI-document 
error = TIXI.tixiCloseDocument( tixiHandle )
if error == 0:
	print "TIXI document closed"

	














