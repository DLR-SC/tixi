#!/usr/bin/python

# Copyright (C) 2007-2011 German Aerospace Center (DLR/SC)

# Created: 2010-08-13 Markus Litz <Markus.Litz@dlr.de>
# Changed: $Id: dbms.h 4577 20xx-xx-xx 09:27:39Z litz_ma $ 

# Version: $Revision: 3978 $

# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at

#     http://www.apache.org/licenses/LICENSE-2.0

# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# @file
# @brief Demo Python script to show how to use TIXI routoines in python scripts
# @author Markus Litz, DLR/SC, 2008-06-10
#

from ctypes import *

# define handles
tixiHandle = c_int(0)
xmlInputFilename    = "howtoin.xml"


# open TIXI and TIGL shared libraries
import sys
if sys.platform == 'win32':
    TIXI = cdll.TIXI
else:
    TIXI = CDLL("libTIXI.so")

# Open a CPACS configuration file. First open the CPACS-XML file
# with TIXI to get a tixi handle.
tixiReturn = TIXI.tixiOpenDocument(xmlInputFilename, byref(tixiHandle))
if tixiReturn != 0:
    print 'Error: tixiOpenDocument failed for file: ' + xmlInputFilename
    exit(1)


#------------------------------------------------------------------------------------------------------------
# now read some xml data from the cpacs file....
#------------------------------------------------------------------------------------------------------------

# text element ...   
planeName = c_char_p()
TIXI.tixiGetTextElement(tixiHandle, "/plane/name", byref(planeName))
print "Plane Name: " + planeName.value

# double element
doubleElement = c_double()
error = TIXI.tixiGetDoubleElement( tixiHandle, "/plane/coordinateOrigin/x", byref(doubleElement) );
if error == 0:
	print "Coordinate origin x-coordinate: " + str(doubleElement.value)


# get the number of wing subelements  of element wings 
parentName = "/plane/wings"
subElementName = "wing"
count = c_int(-1)
   
error = TIXI.tixiGetNamedChildrenCount( tixiHandle, parentName, subElementName, byref(count) );
if error == 0:
    print "Number of wing elements in wings is " + str(count.value) 

	
	
# element /plane/wings has 3 wing elements.  Get the y element of each centerOfGravity element.	
nWings = 3
baseName = "/plane/wings/wing"
subElementName = "/centerOfGravity/y"

# XPath indizes start at 1 !! 
for iWing in range (1, nWings+1):
	elementName = ""
	indexStringSize = 0;
	y = c_double(0.0)
	
	indexString = "[" + str(iWing) + "]"
	elementName = baseName + indexString + subElementName
	
	error = TIXI.tixiGetDoubleElement( tixiHandle, elementName, byref(y) );
	if error == 0:
		print "Center of gravity y coordinate wing " + str(iWing) + ": " +  str(y.value)


# read an attribute 	
# first try to read an non-existing attribute 

position = c_char_p()

# first try to read an non-existing attribute     
error = TIXI.tixiGetTextAttribute( tixiHandle, "/plane/wings/wing[3]", "pstn", byref(position) );
if error == 0:
	print "Position of wing #3 is " + position.value

# Now read an existing attribute. 
error = TIXI.tixiGetTextAttribute( tixiHandle, "/plane/wings/wing[3]", "position", byref(position) );
if error == 0:
	print "Position of wing #3 is " + position.value
	
 
 
# read a 2x3 matrix 
nRows = c_int(-1)
nCols = c_int(-1)
dArray = c_double * 6
array = dArray()
print 
print
print "Reading Matrix..."

# first get the matrix size
error = TIXI.tixiGetMatrixSize( tixiHandle, "/plane/two_by_three_matrix", "blub", "bla", byref(nRows), byref(nCols) )
if error == 0:
	print "Size of matrix 'two_by_three_matrix' is " + str(nRows.value) + "x" +  str(nCols.value) 

#Now get the matrix entries and store them in row first (1) order into array. 

error = TIXI.tixiGetFloatMatrix( tixiHandle, "/plane/two_by_three_matrix", "blub", "bla", nRows, nCols, 1, byref(array) )
if error == 0:
	print "Matrix entries row wise storage: "
	
	for i in range (0, nRows.value):
		for j in range (0, nCols.value):
			print "Row:" + str(i) + ", Col:" + str(j) + " has the value: " + str(array[i*nCols.value+j])
		
		
# Now get the matrix entries and store them in column first (2) order into array. 		
error = TIXI.tixiGetFloatMatrix( tixiHandle, "/plane/two_by_three_matrix", "blub", "bla", nRows, nCols, 2, byref(array) )
if error == 0:
	print
	print "Matrix entries row wise storage: "
	
	for i in range (0, nCols.value):
		for j in range (0, nRows.value):
			print "Col:" + str(i) + ", Row:" + str(j) + " has the value: " + str(array[i*nRows.value+j])


			
# Check elements
print
if TIXI.tixiCheckElement( tixiHandle, "/plane/two_by_three_matrix" ) == 0:
	print "Test passed: Element '/plane/two_by_three_matrix' exists." 
else: 
	print "Test failed: Element '/plane/two_by_three_matrix' does not exist." 
	
if TIXI.tixiCheckElement( tixiHandle, "/plane/MATRIXX" ) != 0:
	print "Test passed: Element '/plane/MATRIXX' does not exist."
else: 
	print "Test failed: Element '/plane/MATRIXX' exists." 


# Check if handle is  valid.
print
print "Checking handle..."
if TIXI.tixiCheckDocumentHandle( tixiHandle ) != 0:
	print "Test failed: Invalid document handle."
else:
	print "Test passed: Document handle is valid"
    
    
#-------------------------------
# Test some vector functions
#-------------------------------
print "Now we read some vector informations"
count = c_int(0)
error = TIXI.tixiGetVectorSize( tixiHandle, "/plane/vectorTest/aVector", byref(count))
if error == 0:
	print "TIXI document closed"

print "Number of elements in vector  = /plane/vectorTest/aVector = " + str(count.value)
 
 
# read the vector
VALUES = c_double * count.value   # $COUNT values
values = VALUES(0 * count.value)

wert = TIXI.tixiGetFloatVector(tixiHandle, "/plane/vectorTest/aVector", byref(values), count.value)
print "RetuenValue = " + str(wert)
print

print "Values of the vector:"
for i in range(1, count.value):
    print "Element " + str(i) + " ==> " + str(values[i])

 
 
# Now we close the document.All memory allocated internally by
# actions on this document is released. This means you have to make
# a copy of all strings returned by TIXI if you want to use them
# after closing the document.   

error = TIXI.tixiCloseDocument(tixiHandle)
if error == 0:
	print "Document closed!"
	print
  
  
  
#------------------------------------------------------------------------------------------------------------
# now demonstrate how to create a new document and adds some contents.
#------------------------------------------------------------------------------------------------------------

xmlOutputFilename   = "howtoout.xml"
handle = c_int(0)

# Create a new document for writing with root element named
# "plane". A file name is attributed to this document on saving.

error = TIXI.tixiCreateDocument( "plane", byref(handle) )
if error != 0:
	# This error is fatal, exit here
	quit()
else:
	print "Created a new xml file."
	
	
# First a header containing the tool name and version and the author
# of the file is added. A timestamp is added automatically, as well. 
error = TIXI.tixiAddHeader( handle, "NoTool", "47.11", "Me" )
if error == 0:
	print "Added header to new xml file."

	
# Now we insert a empty elements into the root element to create a
# hierarchy of elements.
nWings = 6;
error = TIXI.tixiAddTextElement( handle, "/plane", "wings", 0 );
if error == 0:
	print "Inserted empty wings element."

	
# Insert empty wing elements into the wings element. 
for iWing in range(0, nWings):
	error = TIXI.tixiAddTextElement( handle, "/plane/wings", "wing", 0 );
	if error == 0:
		print "Inserted wing elements."

		
		
# Into each wing element insert a position attribute and a wingspan element.
baseName = "/plane/wings/wing"

# XPath indices start at 1 !! 
for iWing in range(1, nWings+1):
	position = ""
	if iWing%2 == 0:
		position = "left"
	else:
		position = "right"
	
	elementName = baseName + "[" + str(iWing) + "]"
	# Add attribute 
	error = TIXI.tixiAddTextAttribute( handle, elementName, "position", position )
	if error == 0:
		print "Added position attribute with value " + position + " to element " + elementName 
    
	# Add wingspan element. 
	span = c_double(iWing + 0.1)
	error = TIXI.tixiAddDoubleElement( handle, elementName, "wingspan", span, "%5.1f" )
	if error == 0:
		print "Added wingspan element with value " + str(span.value) + " to element " + elementName

		
		
		
# write a 4x3 matrix
nRows = 4
nCols = 3
dArray = c_double * (nRows * nCols)
array = dArray()

for i in range(0, nRows):
	for j in range(0, nCols):
		array[i*nCols+j] = i * 10. + j;
   
# Write array to matrix in row order (0)   
error = TIXI.tixiAddFloatMatrix( handle, "/plane", "four_by_three", "rows", "columns", nRows, nCols, 0, array, "%5.1f" )
if error == 0:
	print "4x3 Matrix written"
  
# Now write the array to the matrix in column order (1)
error = TIXI.tixiAddFloatMatrix( handle, "/plane", "four_by_three", "rows", "columns", nRows, nCols, 1, array, "%5.1f" )
if error == 0:
	print "3x4 Matrix written"
		
		

# Create an empty 3x3 matrix and fill the entries with a composite element.
nRows = 3
nCols = 3

# first create an empty matrix
error = TIXI.tixiCreateMatrix( handle, "/plane", "composite", "r", "c", nRows, nCols )	
if error == 0:
	print "3x3 Matrix created."
	
	
	
# Path to a matrix entry is for example
# /plane/composite/r[2]/c[3]
basePath = "/plane/composite/r"

for i in range(1, nRows + 1):
	#build string for i-th the row element path
	rowPath = basePath + "[" + str(i) + "]"
	
	for j in range(1, nCols + 1):
		position = ""
		if (i*j)%2 == 0:
			position = "left"
		else:
			position = "right"
		
		# build string for the path to matrix  entry (i,j)
		entryName = rowPath + "/c[" + str(j) + "]"
	
		# Add an empty wing element to the matrix element (i,j) 
		error = TIXI.tixiAddTextElement( handle, entryName, "wing", 0 )
		if error == 0:
			print "entryName added."
	
		wingPath = entryName + "/wing"
		# Insert a position and a span element into the wing element 
		error = TIXI.tixiAddTextElement( handle, wingPath, "position", position )
		if error == 0:
			print "wingPath/position added."

		span = c_double((i+j) * 10.)
		error = TIXI.tixiAddDoubleElement( handle, wingPath, "span", span, 0 )
		if error == 0:
			print "wingPath/span added."
	
    
# Now write a vector 
CArray = c_double * 10
array = CArray(1,4,5.8,77.0,5,6,7,8,9,10)
error = TIXI.tixiAddFloatVector( handle, "/plane", "testVector", array, 10)
if error != 0:
    print 'Error in writing a vector with TIXI'
    exit(1)


		
#  After all elements have been added, save the document to file 
error = TIXI.tixiSaveDocument( handle, xmlOutputFilename )
if error == 0:
	print "Document is written."
 
#  Now we can close the document 
error = TIXI.tixiCloseDocument( handle )
if error == 0:
	print "TIXI document closed"

	
    
    
#-------------------------------
# import a xml string to tixi
#-------------------------------

tixiImportString = "<wings><b>3</b></wings>"
tixiReturn = TIXI.tixiImportFromString(tixiImportString, byref(tixiHandle))
if tixiReturn != 0:
    print 'Error: tixiImportFromString failed for string: ' + tixiImportString
    exit(1)
else:
	print "String imported"
		
print
# export document as string
xmlDocumentString = c_char_p()
TIXI.tixiExportDocumentAsString(tixiHandle, byref(xmlDocumentString))
print "---------------------------------"
print "Plane Name: " + xmlDocumentString.value
print "---------------------------------"













