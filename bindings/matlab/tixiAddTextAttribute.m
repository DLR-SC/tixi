function tixiAddTextAttribute(handle, xpath, attribName, attribValue)
    if (ischar(handle))
        error('Invalid handle');
    end
    
    if not(ischar(xpath))
        error('Invalid xpath');
    end
    
    if not(ischar(attribName))
        error('Invalid attribute name');
    end
    
    if not(ischar(attribValue))
        error('Invalid attribute value');
    end
    
    try 
        tixi_matlab('tixiAddTextAttribute',handle, xpath, attribName, attribValue);
    catch  err
        error(err.message);
    end
end