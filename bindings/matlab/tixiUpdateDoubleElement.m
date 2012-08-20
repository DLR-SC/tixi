function tixiUpdateDoubleElement(handle, xpath, value, format)
    if (ischar(handle))
        error('Invalid handle');
    end
    
    if not(ischar(xpath))
        error('Invalid xpath');
    end
    
    if ischar(value)
        error('Invalid value');
    end
    
    if not(ischar(format))
        error('Invalid format argument');
    end
    
    try 
        tixi_matlab('tixiUpdateDoubleElement',handle, xpath, value, format);
    catch  err
        error(err.message);
    end
end