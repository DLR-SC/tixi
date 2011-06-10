/* 
* Copyright (C) 2007-2011 German Aerospace Center (DLR/SC)
*
* Created: 2010-08-13 Markus Litz <Markus.Litz@dlr.de>
* Changed: $Id: TIXIInterfaceStatusCodes.java 3 2011-02-09 14:50:00Z markus.litz $ 
*
* Version: $Revision: 3 $
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

import java.util.Hashtable;
import java.util.Map;


/**
 * 
 * Enum for the TIXI Return Codes.
 *
 * @version $LastChangedRevision: 4272 $
 * @author Markus Litz
 */
public enum TIXIInterfaceStatusCodes {

    /**
     * The TIXI status codes - 2009-03-12.
     */
    SUCCESS(0),                         /** !< No error occurred */
    FAILED(1),                          /** !< Unspecified error */
    INVALID_XML_NAME(2),                /** !< Non XML standard complaint name specified */
    NOT_WELL_FORMED(3),                 /** !< Document is not well formed */
    NOT_SCHEMA_COMPLIANT(4),            /** !< Document is not schema complaint */
    NOT_DTD_COMPLIANT(5),               /** !< Document is not DTD complaint */
    INVALID_HANDLE(6),                  /** !< Document handle is not valid */
    INVALID_XPATH(7),                   /** !< XPath expression is not valid */
    ELEMENT_NOT_FOUND(8),               /** !< Element does not exist in document */
    INDEX_OUT_OF_RANGE(9),              /** !< Index supplied as argument is not inside the admissible range */
    NO_POINT_FOUND(10),                 /** !< No point element found a given XPath */
    NOT_AN_ELEMENT(11),                 /** !< XPath expression does not point to an XML-element node */
    ATTRIBUTE_NOT_FOUND(12),            /** !< Element does not have the attribute */
    OPEN_FAILED(13),                    /** !< Error on opening the file */
    OPEN_SCHEMA_FAILED(14),             /** !< Error on opening the schema file */
    OPEN_DTD_FAILED(15),                /** !< Error on opening the DTD file */
    CLOSE_FAILED(16),                   /** !< Error on closing the file */
    ALREADY_SAVED(17),                  /** !< Trying to modify already saved document */
    ELEMENT_PATH_NOT_UNIQUE(18),        /** !< Path expression can not be resolved unambiguously */
    NO_ELEMENT_NAME(19),                /** !< Element name argument is NULL */
    NO_CHILDREN(20),                    /** !< Node has no children */
    CHILD_NOT_FOUND(21),                /** !< Named child is not child of element specified */
    EROROR_CREATE_ROOT_NODE(22),        /** !< Error when adding root node to new document   */
    DEALLOCATION_FAILED(23),            /** !< On closing a document the deallocation of allocated memory fails */
    NO_NUMBER(24),                      /** !< No number specified */
    NO_ATTRIBUTE_NAME(25),              /** !< No attribute name specified */
    STRING_TRUNCATED(26),               /** !< String variable supplied is to small to hold the result, Fortran only */
    NON_MATCHING_NAME(27),              /** !< Row or column name specified do not match the names used in the document */
    NON_MATCHING_SIZE(28),              /** !< Number of rows or columns specified do not match the sizes of the matrix in the document */
    MATRIX_DIMENSION_ERROR(29),         /** !< if nRows or nColumns or both are less than 1 */
    COORDINATE_NOT_FOUND(30),           /** !< missing coordinate inside a point element */
    UNKNOWN_STORAGE_MODE(31);           /** !< storage mode specified is neither ROW_WISE nor COLUMN_WISE  */

    
    private static final Map<Integer, TIXIInterfaceStatusCodes> ERRORCODES = new Hashtable<Integer, TIXIInterfaceStatusCodes>();

    static {    // quick access map statically initialized AFTER implicit enum constructor (!)
        ERRORCODES.put(Integer.valueOf(0), SUCCESS);
        ERRORCODES.put(Integer.valueOf(1), FAILED);
        ERRORCODES.put(Integer.valueOf(2), INVALID_XML_NAME);
        ERRORCODES.put(Integer.valueOf(3), NOT_WELL_FORMED);
        ERRORCODES.put(Integer.valueOf(4), NOT_SCHEMA_COMPLIANT);
        ERRORCODES.put(Integer.valueOf(5), NOT_DTD_COMPLIANT);
        ERRORCODES.put(Integer.valueOf(6), INVALID_HANDLE);
        ERRORCODES.put(Integer.valueOf(7), INVALID_XPATH);
        ERRORCODES.put(Integer.valueOf(8), ELEMENT_NOT_FOUND);
        ERRORCODES.put(Integer.valueOf(9), INDEX_OUT_OF_RANGE);
        ERRORCODES.put(Integer.valueOf(10), NO_POINT_FOUND);
        ERRORCODES.put(Integer.valueOf(11), NOT_AN_ELEMENT);
        ERRORCODES.put(Integer.valueOf(12), ATTRIBUTE_NOT_FOUND);
        ERRORCODES.put(Integer.valueOf(13), OPEN_FAILED);
        ERRORCODES.put(Integer.valueOf(14), OPEN_SCHEMA_FAILED);
        ERRORCODES.put(Integer.valueOf(15), OPEN_DTD_FAILED);
        ERRORCODES.put(Integer.valueOf(16), CLOSE_FAILED);
        ERRORCODES.put(Integer.valueOf(17), ALREADY_SAVED);
        ERRORCODES.put(Integer.valueOf(18), ELEMENT_PATH_NOT_UNIQUE);
        ERRORCODES.put(Integer.valueOf(19), NO_ELEMENT_NAME);
        ERRORCODES.put(Integer.valueOf(20), NO_CHILDREN);
        ERRORCODES.put(Integer.valueOf(21), CHILD_NOT_FOUND);
        ERRORCODES.put(Integer.valueOf(22), EROROR_CREATE_ROOT_NODE);
        ERRORCODES.put(Integer.valueOf(23), DEALLOCATION_FAILED);
        ERRORCODES.put(Integer.valueOf(24), NO_NUMBER);
        ERRORCODES.put(Integer.valueOf(25), NO_ATTRIBUTE_NAME);
        ERRORCODES.put(Integer.valueOf(26), STRING_TRUNCATED);
        ERRORCODES.put(Integer.valueOf(27), NON_MATCHING_NAME);
        ERRORCODES.put(Integer.valueOf(28), NON_MATCHING_SIZE);
        ERRORCODES.put(Integer.valueOf(29), MATRIX_DIMENSION_ERROR);
        ERRORCODES.put(Integer.valueOf(30), COORDINATE_NOT_FOUND);
        ERRORCODES.put(Integer.valueOf(31), UNKNOWN_STORAGE_MODE);
    }

    /**
     * Declaration of the enum property.
     */
    private final int errorCode;

    /**
     * Enum constructor.
     * @param returnValue The code to associate with the enum label
     */
    private TIXIInterfaceStatusCodes(final int returnValue) {
        errorCode = returnValue;
    }

    /**
     * Accessor for the enums .getError() method.
     * @param eCode the TIXI error code
     * @return The tixi errorcode enum
     */
    public static TIXIInterfaceStatusCodes getError(final int eCode) {
        return ERRORCODES.get(Integer.valueOf(eCode));
    }
    
    /**
     * To suppress warning.
     * @return The integer code
     */
    public int getCode() {
        return errorCode;
    }

}
