#!/usr/bin/python
"""
Copyright (C) 2007-2020 German Aerospace Center (DLR/SC)

Created: 2010-08-13 Markus Litz <Markus.Litz@dlr.de>

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

@file
@brief Demo Python script to show how to use TIXI routoines in python scripts
@author Markus Litz, DLR/SC, 2008-06-10
"""

from tixi3 import tixi3wrapper
from tixi3.tixi3wrapper import Tixi3Exception

# define handles and open the tixi document
tixi_handle = tixi3wrapper.Tixi3()
tixi_handle.open("../Demo/howtoin.xml")

# ----------------------------------------------------------------------------
# now read some xml data from a file....
# ----------------------------------------------------------------------------

# text element ...   
print(tixi_handle.getTextElement("/plane/name"))

# double element
print(tixi_handle.getDoubleElement("/plane/coordinateOrigin/x"))

# get the number of wing subelements  of element wings 
parent_name = "/plane/wings"
sub_element_name = "wing"
n_wings = tixi_handle.getNamedChildrenCount(parent_name, sub_element_name)
print("Number of wing elements in wings is {}.".format(n_wings))

# element /plane/wings has 3 wing elements.  Get the y element of each centerOfGravity element.
base_name = "/plane/wings/wing"

# xpath indices start at 1 !!
for i_wing in range(1, n_wings + 1):
    element_name = "/plane/wings/wing[{}]/centerOfGravity/y".format(i_wing)
    y = tixi_handle.getDoubleElement(element_name)
    print("Center of gravity y coordinate wing {}: {}".format(i_wing, y))

# read an attribute

# first try to read an non-existing attribute
sub_element_name = "pstn"
try:
    position = tixi_handle.getTextAttribute("/plane/wings/wing[3]", sub_element_name)
    print("Position of wing #3 is " + position + ".")
except Tixi3Exception as e:
    print("TiXI threw an \"{}\" exception when trying to read from sub_element \"{}\"!"
          .format(e.error, sub_element_name))

# Now read an existing attribute. Proper exception handling is strongly encouraged!
sub_element_name = "position"
try:
    position = tixi_handle.getTextAttribute("/plane/wings/wing[3]", sub_element_name)
    print("Position of wing #3 is " + position)
except Tixi3Exception as e:
    print("TiXI threw an \"{}\" exception when trying to read from sub_element \"{}\"!"
          .format(e.error, sub_element_name))

# Check elements
if tixi_handle.checkElement("/plane/two_by_three_matrix"):
    print("Test passed: Element '/plane/two_by_three_matrix' exists.")
else:
    print("Test failed: Element '/plane/two_by_three_matrix' does not exist.")

if not tixi_handle.checkElement("/plane/MATRIXX"):
    print("Test passed: Element '/plane/MATRIXX' does not exist.")
else:
    print("Test failed: Element '/plane/MATRIXX' exists.")

# Check if handle is  valid.
print("\nChecking handle...")
if tixi_handle.checkDocumentHandle():
    print("Test failed: Invalid document handle.")
else:
    print("Test passed: Document handle is valid")

# -------------------------------
# Test some vector functions
# -------------------------------

print("\nNow we read some vector information")
count = tixi_handle.getVectorSize("/plane/vectorTest/aVector")
print("Number of elements in vector  = /plane/vectorTest/aVector = " + str(count))

# read the vector
vector = tixi_handle.getFloatVector("/plane/vectorTest/aVector", count)
print("vector entries: {}".format(vector))

# Now we close the document.
tixi_handle.close()

# ------------------------------------------------------------------------------------------------------------
# now demonstrate how to create a new document and adds some contents.
# ------------------------------------------------------------------------------------------------------------

xml_output_filename = "howtoout.xml"
tixi_handle = tixi3wrapper.Tixi3()

# Create a new document for writing with root element named
# "plane". A file name is attributed to this document on saving.
tixi_handle.create("plane")

# First a header containing the tool name and version and the author
# of the file is added. A timestamp is added automatically, as well.
tixi_handle.addHeader("NoTool", "47.11", "Me")

# Now we insert empty elements into the root element to create a
# hierarchy of elements.
n_wings = 6
tixi_handle.addTextElement("/plane", "wings", 0)

# Insert empty wing elements into the wings element.
for i_wing in range(0, n_wings):
    tixi_handle.addTextElement("/plane/wings", "wing", 0)

# Into each wing element insert a position attribute and a wingspan element.
base_name = "/plane/wings/wing"

# XPath indices start at 1 !!
for i_wing in range(1, n_wings+1):

    if i_wing % 2 == 0:
        position = "left"
    else:
        position = "right"

    element_name = base_name + "[" + str(i_wing) + "]"

    # Add attribute
    tixi_handle.addTextAttribute(element_name, "position", position)

    # Add wingspan element.
    span = i_wing + 0.1
    tixi_handle.addDoubleElement(element_name, "wingspan", span, "%5.1f")

# Now write a vector
array = [1, 4, 5.8, 77.0, 5, 6, 7, 8, 9, 10]
tixi_handle.addFloatVector("/plane", "testVector", array, 10, "%5.1f")

#  After all elements have been added, save the document to file
tixi_handle.save(xml_output_filename)

#  Now we can close the document
tixi_handle.close()

# -------------------------------
# import a xml string to tixi
# -------------------------------

tixi_import_string = "<wings><b>3</b></wings>"
tixi_handle.openString(tixi_import_string)

# export document as string
xml_document_string = tixi_handle.exportDocumentAsString()
print("---------------------------------")
print(xml_document_string)
print("---------------------------------")
