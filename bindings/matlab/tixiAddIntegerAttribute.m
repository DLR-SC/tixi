function tixiAddIntegerAttribute(handle, xpath, attribName, attribValue, formatString)
    if (ischar(handle))
        error('Invalid handle');
    end
    
    if not(ischar(xpath))
        error('Invalid xpath');
    end
    
    if not(ischar(attribName))
        error('Invalid attribute name');
    end
    
    if ischar(attribValue)
        error('Invalid attribute value');
    end
    
        
    if not(ischar(formatString))
        error('Invalid format string');
    end
    
    try 
        tixi_matlab('tixiAddIntegerAttribute',handle, xpath, attribName, attribValue, formatString);
    catch  err
        error(err.message);
    end
end