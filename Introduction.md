# What is TIXI #
Based on these requirements it was decided to design a library adhering to an API that supports developers while accessing and archiving their data via a simple interface for operations on the underlying XML structure.
On top of the popular and stable XML-library libXML2 (http://www.xmlsoft.org), the C-library TIXI was designed to provide a simple but powerful Interface to XML processing specialist for the needs of Engineers. So TIXI should help on XML processing when performing simple tasks like writing an integer or reading a matrix or vector.
In most cases only the XPath to the location inside the XML tree  representation and/or the corresponding data is required to read or write data. Language interfaces and example applications for Fortran and Python are also provided.



# XML and XPath #
The string to describe a path to an element has to be an XPath expression but only a very restricted subset of XPath expressions are allowed: On retrieving information the specified path has to be unique. If an element name appears more than once inside the same parent element (see wings/wing in the example) the expression has to be contain an index to denominate exactly which element has to be used. As long as these restrictions are met any syntactically correct XPath expression is allowed. When adding an element the complete path has to be specified.

As Example:
```
<?xml version="1.0" encoding="UTF-8"?>
 <plane>
     <name>Junkers JU 52</name>
     <wings>
         <wing position='left'>
             <centerOfGravity referenceSystem='relative'>
                 <x unit="m">30.0</x>
                 <y unit="m">10.0</y>
                 <z unit="m">5.0</z>
             </centerOfGravity>
         </wing>
         <wing position='right'>
             <centerOfGravity referenceSystem='relative'>
                 <x unit="m">30.0</x>
                 <y unit="m">-10.0</y>
                 <z unit="m">5.0</z>
             </centerOfGravity>
         </wing>
     </wings>
     <coordinateOrigin>
         <x unit="m">0.0</x>
         <y unit="m">0.0</y>
         <z unit="m">0.0</z>
     </coordinateOrigin>
 </plane>
```


The path to the x coordinate value of the center of gravity of the plane is: `/plane/centerOfGravity/x`

The path to the x coordinate value of the center of gravity of the first wing element is: `/plane/wings/wing[1]/centerOfGravity/x`

The path to the name can either by expressed as `/plane/name` or `//name` which would select all elements called name but as long as this element name is unique in the document it is valid in the context of TIXI.

None valid expressions are: `/plane/wings/wing/centerOfGravity/z` is not
unique because there are two wing elements in wings.

`/plane/coordinateOrigin` specifies an element with children containing text.

`//x` is not unique because it would select all elements named `x` as there are:
  * `/plane/wings/wing[1]/centerOfGravity/x`
  * `/plane/wings/wing[2]/centerOfGravity/x`
  * `/plane/centerOfGravity/x`