#!/usr/bin/python
#
# @file
# @brief Demo Python script to show how TIXI opens a xml file and includes all 
#             XML files that are linked in.
# @author Markus Litz, DLR/SC, 2008-06-23
#

from tixi3 import tixi3wrapper

# define handles
tixi = tixi3wrapper.Tixi3()
xmlInputFilename = "MainFile.xml"
xmlOutputFilename = "Result_out.xml"

# Open a CPACS configuration file. First open the CPACS-XML file
# with TiXI to get a tixi handle.
tixi.open(xmlInputFilename, recursive=True)

# remove an Element from the main xml file:
tixi.removeElement("/plane/aPoint")

# add an element to a node that is located in a external satellite file
tixi.addTextElement("/plane/testNode/aVeryTest/testNode/aVeryTest3/point", "y", "123.45")

# export document as string

xmlDocumentString = tixi.exportDocumentAsString()
print("---------------------------------")
print(xmlDocumentString)
print("---------------------------------")

#  After all elements have been edited, save the document to file with all changes in the sub-files.
tixi.saveDocument(xmlOutputFilename)
 
#  Now we can close the TiXI-document
error = tixi.close()















