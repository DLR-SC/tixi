/* 
* Copyright (C) 2007-2011 German Aerospace Center (DLR/SC)
*
* Created: 2010-08-13 Markus Litz <Markus.Litz@dlr.de>
* Changed: $Id$ 
*
* Version: $Revision$
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

package de.dlr.sc.chameleon.rce.tixi;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Pointer;
import com.sun.jna.ptr.DoubleByReference;
import com.sun.jna.ptr.IntByReference;
import com.sun.jna.ptr.PointerByReference;


/**
 * This class is a JNA-Wrapper around the TIXI library.
 *
 * @version $LastChangedRevision$
 * @author Markus Litz
 * @author Arne Bachmann
 */
public class TIXIInterface {
    
    /**
     * Central logger instance.
     */
    protected static final Log LOGGER = LogFactory.getLog(TIXIInterface.class);

    static {
        String operatingSystem = System.getProperty("os.name");
        if (operatingSystem.startsWith("Windows")) {
        	// don't change import order, works just fine now
            System.loadLibrary("msvcr71d");
            System.loadLibrary("libidn-11");
            System.loadLibrary("libeay32");
            System.loadLibrary("libssl32");
            System.loadLibrary("libcurl");
            System.loadLibrary("libssh2");
            System.loadLibrary("zlib1");
            System.loadLibrary("libcurld");
            System.loadLibrary("ssleay32");
            System.loadLibrary("iconv");
            System.loadLibrary("libxml2");
            System.loadLibrary("libxslt");
            System.loadLibrary("libexslt");
            System.loadLibrary("libsasl");
            System.loadLibrary("openldap");
            System.loadLibrary("curllib");
            System.loadLibrary("TIXI");
        } else { // Linux
            System.loadLibrary("z");
            System.loadLibrary("xml2");
            System.loadLibrary("xslt");
            System.loadLibrary("curl");
            System.loadLibrary("TIXI");
        }
        LOGGER.info("TIXI Libraries loaded");

    }

    /**
     *  Internal TIXI handler.
     */
    private IntByReference tixiHandle = null;
    
    /**
     *  The return value from TIXI.
     */
    private int errorCode = 0;

    
    /**
     * Constructor.
     */
    public TIXIInterface() {
    }

    /**
     * 
     * Creates an element specified by the elementPath expression and insert text into the element.
     * Elements with the same name can be added multiple times.
     * 
     * @param parentPath
     *            an XPath complaint path to an element in the loaded document into which the new
     *            element is to be inserted. The parent element has to exist already.
     * @param elementName
     *            name of the element to be inserted into the parent element
     * @param text
     *            text to be placed inside the element pointed to by elementPath. If text is NULL an
     *            empty element will be created.
     * @return true if success.
     */
    public boolean tixiAddTextElement(final String parentPath, final String elementName, final String text) {
        if ((parentPath.equals("")) || (elementName.equals(""))) {
            LOGGER.error("parentPath or elementName empty in tixiAddTextElement::TIXIInterface");
            return false;
        }

        errorCode = TIXI.INSTANCE.tixiAddTextElement(tixiHandle.getValue(), parentPath, elementName, text);
        if (errorCode != 0) {
            LOGGER.error("tixiAddTextElement failed in TIXIInterface");
            return false;
        }
        return true;
    }
    
    /**
     * 
     * More user convenience function of tixiAddTextElement. 
     * 
     * @param elementPath - The XPath where to create a new text element.
     * @param text The value of the new text element.
     * @return True if success.
     */
    public boolean tixiAddTextElement(final String elementPath, final String text) {
        if ((elementPath.equals(""))) {
            LOGGER.error("elementPath empty in tixiAddTextElement::TIXIInterface");
            return false;
        }
        
        // extract element name
        int pos = elementPath.lastIndexOf("/");
        final String pathHelper = elementPath.substring(0, pos);
        final String nameHelper = elementPath.substring(pos + 1);

        return tixiAddTextElement(pathHelper, nameHelper, text);
    }
    
    
    
    /**
     * 
     * Returns the value of an attribute specified by attributeName of the
     * element, specified by elementPath, in the document specified by
     * handle. On successful return the memory used for value is allocated
     * internally and must not be released by the user. The memory is
     * deallocated when the document referred to by handle is closed.
     * 
     * @param elementPath - an XPath complaint path to an element in the document
     * @param attributeName - name of the attribute to be get from the element
     * @return value of the specified attribute as a string
     */
    public String tixiGetTextAttribute(final String elementPath, final String attributeName) {
        PointerByReference pRef = new PointerByReference();
        errorCode = TIXI.INSTANCE.tixiGetTextAttribute(tixiHandle.getValue(), elementPath, attributeName, pRef);
        
        if (errorCode != 0) {
            LOGGER.error("tixiGetTextAttribute failed in TIXIInterface");
            return "";
        }
        
        Pointer pointer = pRef.getValue();
        return pointer.getString(0);
    }
    
    
    
    /**
     * Cleanup TIXI stuff. Force TIXI to free memory. 
     */
    public void tixiCloseDocument() {
        if (tixiHandle == null) {
            LOGGER.error("Tried to close tixi document whose handle is null (not loaded/imported before)");
            return;
        }
        errorCode = TIXI.INSTANCE.tixiCloseDocument(tixiHandle.getValue());
        if (errorCode != 0) {
            LOGGER.error("tixiCloseDocument failed in TIXIInterface");
        } 
        tixiHandle = null;
    }
    
    /**
     * Returns a String with the XML content of the TIXI-document.
     * @return A String with the full XML content.
     */
    public String tixiExportDocumentAsString() {
     // exit if this instance has already a tixi handle
        if (tixiHandle == null) {
            return null;
        }
        
        final PointerByReference pRef = new PointerByReference();
        errorCode = TIXI.INSTANCE.tixiExportDocumentAsString(tixiHandle.getValue(), pRef);
        
        if (errorCode != 0) {
            LOGGER.error("tixiExportDocumentAsString failed in TIXIInterface");
            return null;
        }
        
        final Pointer pointer = pRef.getValue();
        return pointer.getString(0);
    }
    
    /**
     * 
     * Returns a double Element selected by XPath.
     * 
     * @param elementPath The XPath to the XML-Element to read.
     * @return The double value of the Element.
     */
    public double tixiGetDoubleElement(final String elementPath) {
        final DoubleByReference doubleElement = new DoubleByReference();
        errorCode = TIXI.INSTANCE.tixiGetDoubleElement(tixiHandle.getValue(), elementPath, doubleElement);
        
        if (errorCode != 0) {
            LOGGER.error("tixiGetDoubleElement failed in TIXIInterface");
        }
        return doubleElement.getValue();
    }

    
    /**
     * Returns the number of children elements with the same name.
     * 
     * @param elementPath - The XPath to the XML-Element to read.
     * @param childName - name of children to be counted
     * @return number of children with name childName.
     */
    public int tixiGetNamedChildrenCount(final String elementPath, final String childName) {
        final IntByReference intElement = new IntByReference();
        errorCode = TIXI.INSTANCE.tixiGetNamedChildrenCount(tixiHandle.getValue(), elementPath, childName, intElement);
        
        if (errorCode != 0) {
            LOGGER.error("tixiGetNamedChildrenCount failed in TIXIInterface");
        }
        
        return intElement.getValue();
    }
    
    
    
    /**
     * 
     * Returns a text Element selected by XPath.
     * 
     * @param elementPath The XPath to the XML-Element to read.
     * @return The text value of the Element.
     */
    public String tixiGetTextElement(final String elementPath) {
        final PointerByReference pRef = new PointerByReference();
        errorCode = TIXI.INSTANCE.tixiGetTextElement(tixiHandle.getValue(), elementPath, pRef);
        
        if (errorCode != 0) {
            LOGGER.error("tixiGetTextElement failed in TIXIInterface");
            return "";
        }
        
        final Pointer pointer = pRef.getValue();
        return pointer.getString(0);
    }
    
    
    
    /**
     * Changes the value of a XML node.
     * 
     * @param elementPath - the path to the node
     * @param newTextValue - the new value
     * @return true if success
     */
    public boolean tixiUpdateTextElement(final String elementPath, final String newTextValue) {
        if (elementPath.equalsIgnoreCase("") || newTextValue.equalsIgnoreCase("")) {
            return false;
        }
        
        errorCode = TIXI.INSTANCE.tixiUpdateTextElement(tixiHandle.getValue(), elementPath, newTextValue);
        if (errorCode != 0) {
            LOGGER.error("tixiUpdateTextElement failed in TIXIInterface");
            return false;
        }
        return true;
    }
    
    
    /**
     * 
     * Removes an element specified by XPath.
     * 
     * @param elementPath The path to the element to remove 
     * @return True if success.
     */
    public boolean tixiRemoveElement(final String elementPath) {
        if (elementPath.equalsIgnoreCase("")) {
            return false;
        }

        errorCode = TIXI.INSTANCE.tixiRemoveElement(tixiHandle.getValue(), elementPath);
        if (errorCode != 0) {
            LOGGER.error("tixiRemoveElement failed in TIXIInterface.");
            return false;
        }
        return true;
    }
    

    /**
     * 
     * Returns a text Element selected by XPath. It doen't matter what kind of node is selected 
     * through the XPath.  
     * 
     * @param xpathExpression The XPath Expression to evaluate
     * @param index The (index)-number to get. Needed if the XPath expression catches more than one node. Index must
                                      be between 0 and 'tixiXPathEvaluateNodeNumber()'.
     * @return The text value of the Element.
     */
    public String tixiXPathExpressionGetTextByIndex(final String xpathExpression, final int index) {
        PointerByReference pRef = new PointerByReference();
        errorCode = TIXI.INSTANCE.tixiXPathExpressionGetTextByIndex(tixiHandle.getValue(), xpathExpression, index, pRef);
        
        if (errorCode != 0) {
            LOGGER.error("tixiXPathExpressionGetTextByIndex failed in TIXIInterface");
            return "";
        }
        
        Pointer pointer = pRef.getValue();
        return pointer.getString(0);
    }
    
    /**
     * 
     * Imports a XML-String to use with TIXI.
     * 
     * @param xmlImportString The XML-String to use with tixi.
     * @return true if success.
     */
    public boolean tixiImportFromString(final String xmlImportString) {
        boolean success = true;

        // exit if this instance has already a tixi handle
        if (tixiHandle != null) {
            LOGGER.warn("Already called tixiImportFromString.");
            return false;
        }

        // check String
        if ((xmlImportString == null) || xmlImportString.equals("")) {
            LOGGER.error("Filename is null or empty in tixiImportFromString.");
            return false;   // unsuccessful
        }
        
        tixiHandle = new IntByReference();
        errorCode = TIXI.INSTANCE.tixiImportFromString(xmlImportString, tixiHandle);
        if (errorCode != 0) {
            LOGGER.error("tixiImportFromString failed in TIXIInterface " + Integer.toString(errorCode));
            success = false;
        }
        return success;
    }
    

    /**
     * Open a file with TIXI.
     * 
     * @param fileName The full path to a XML file to open with tixi.
     * @param openMode The openMode to use. 0 = plain -- 1 = recursive.
     * @return true if success.
     */
    public boolean tixiOpenDocument(final String fileName, final int openMode) {
        boolean retVal = true;
        // exit if this instance has already a tixi handle
        if (tixiHandle != null) {
            retVal = false;
        }
        tixiHandle = new IntByReference();

        errorCode = TIXI.INSTANCE.tixiOpenDocumentRecursive(fileName, tixiHandle, openMode);        
        if (errorCode != 0) {
            LOGGER.error("tixiOpenDocument failed in TIXIInterface");
            retVal = false;
        }
        return retVal;
    }
    
    /**
     * Save a CPACS document.
     * @param xmlFilename The filename to save under
     * @return Success
     */
    public boolean tixiSaveDocument(final String xmlFilename) {
        boolean leave = false;
        if (xmlFilename.equals("")) {
            LOGGER.error("Filename empty in tixiSaveDocument.");
            leave = true;
        }
        if (leave || tixiHandle == null) {
            return false;
        }
        
        errorCode = TIXI.INSTANCE.tixiSaveDocument(tixiHandle.getValue(), xmlFilename);
        if (errorCode != 0) {
            LOGGER.error("tixiSaveDocument failed in TIXIInterface.");
            return false;
        }
        return true;        
    }
    
    /**
     * Save the complete document into one XML file instead of several files (?).
     * @param xmlFilename The filename to save under
     * @return Success
     */
    public boolean tixiSaveCompleteDocument(final String xmlFilename) {
        boolean leave = false;
        if (xmlFilename.equals("")) {
            LOGGER.error("Filename empty in tixiSaveCompleteDocument.");
            leave = true;   // to avoid checkstyle warning
        }
        if (leave || tixiHandle == null) {
            return false;
        }
        
        errorCode = TIXI.INSTANCE.tixiSaveCompleteDocument(tixiHandle.getValue(), xmlFilename);
        if (errorCode != 0) {
            LOGGER.error("tixiSaveDocument failed in TIXIInterface");
            return false;
        }
        return true;        
    }
    
    /**
     * Remove included files and save the document .
     * @param xmlFilename The filename to save under
     * @return Success
     */
    public boolean tixiSaveAndRemoveDocument(final String xmlFilename) {
        boolean leave = false;
        if (xmlFilename.equals("")) {
            LOGGER.error("Filename empty in tixiSaveAndRemoveCompleteDocument.");
            leave = true;   // to avoid checkstyle warning
        }
        if (leave || tixiHandle == null) {
            return false;
        }
        
        errorCode = TIXI.INSTANCE.tixiSaveAndRemoveDocument(tixiHandle.getValue(), xmlFilename);
        if (errorCode != 0) {
            LOGGER.error("tixiSaveDocument failed in TIXIInterface");
            return false;
        }
        return true;        
    }
    
    /**
     * Helper function to get exact error code of last unsuccessful call.
     * @return The status enum label
     */
    public TIXIInterfaceStatusCodes getLastError() {
        return TIXIInterfaceStatusCodes.getError(errorCode);
    }

    /**
     * Validate the given file.
     * 
     * @param schemaFileName - the name of the file containing the schema to validate against.
     * @return True if valid
     */
    public boolean tixiSchemaValidateFromFile(final String schemaFileName) {
        if (tixiHandle != null) {
            final int code = TIXI.INSTANCE.tixiSchemaValidateFromFile(tixiHandle.getValue(), schemaFileName);
            if (code == TIXIInterfaceStatusCodes.SUCCESS.getCode()) {
                return true;
            }
            LOGGER.error("Error in CPACS validation: TIXI returns " + code);
            return false;
        }
        LOGGER.error("TIXI handle is null, could not validate");
        return false;
    }
    
    
    /**
     * Validate the document against a schema file.
     * 
     * @param xsdFileContents - a String containing the schema to validate against.
     * @return True if valid
     */
    public boolean tixiSchemaValidateFromString(final String xsdFileContents) {
        if (tixiHandle != null) {
            final int code = TIXI.INSTANCE.tixiSchemaValidateFromString(tixiHandle.getValue(), xsdFileContents);
            if (code == TIXIInterfaceStatusCodes.SUCCESS.getCode()) {
                return true;
            }
            LOGGER.error("Error in CPACS validation: TIXI returns " + code);
            return false;
        }
        LOGGER.error("TIXI handle is null, could not validate");
        return false;
    }
    
    /**
     * Returns the TIXI handle.
     * @return the tixi handle.
     */
    public IntByReference getTixiHandle() {
        return tixiHandle;
    }
    
    /**
     * 
     * Returns the Version number of TIXI as a Java String.
     * @return a String containing the version number of TIXI.
     */
    public String tixiGetVersion() {
        return TIXI.INSTANCE.tixiGetVersion();
    }
    
    /**
     * Interface to communicate with TIXI. For detailed documentation see "tixi.h" c-header file! 
     */
    public interface TIXI extends Library {
        TIXI INSTANCE = (TIXI) Native.loadLibrary("TIXI", TIXI.class);
        int tixiAddTextElement(final int handle, String parentPath, String elementName, String text);
        int tixiCloseDocument(final int tixiHandle);
        int tixiExportDocumentAsString(final int tixiHandle, final PointerByReference text);
        int tixiGetDoubleElement(final int tixiHandle, final String elementPath, final DoubleByReference doubleElement);
        int tixiGetTextElement(final int tixiHandle, final String elementPath, final PointerByReference text);
        int tixiGetTextAttribute(final int tixiHandle, final String elementPath, final String attributeName, final PointerByReference text);
        int tixiGetNamedChildrenCount(final int tixiHandle, final String elementPath, final String childName, final IntByReference count);
        int tixiUpdateTextElement (final int tixiHandle, final String elementPath, final String newTextValue);
        int tixiImportFromString(final String xmlImportString, final IntByReference tixiHandle);
        int tixiOpenDocument(final String fileName, final IntByReference tixiHandle);
        int tixiOpenDocumentRecursive(final String fileName, final IntByReference tixiHandle, final int openMode);
        int tixiRemoveElement(final int tixiHandle, final String elementPath);
        int tixiSaveDocument(final int tixiHandle, final String xmlFilename);
        int tixiSaveCompleteDocument(final int tixiHandle, final String xmlFilename);
        int tixiSaveAndRemoveDocument(final int tixiHandle, final String xmlFilename);
        int tixiSchemaValidate(final int tixiHandle, final String xmlFilename);
        int tixiSchemaValidateFromString(final int tixiHandle, final String xsdFileContents);
        int tixiXPathExpressionGetTextByIndex(final int tixiHandle, final String xPathExpression, final int elementNumber, final PointerByReference text);
        int tixiSchemaValidateFromFile(final int tixiHandle, final String schemaFileName);
        String tixiGetVersion(); 
    }

}
