function tixiAddIntegerElement(handle, xpath, name, ivalue, format)
    if (ischar(handle))
        error('Invalid handle');
    end
    
    if not(ischar(xpath))
        error('Invalid xpath');
    end
    
    if not(ischar(name))
        error('Invalid tag name');
    end
    
    if ischar(ivalue)
        error('Invalid value');
    end
    
    if not(ischar(format))
        error('Invalid format argument');
    end
    
    try 
        tixi_matlab('tixiAddIntegerElement',handle, xpath, name, ivalue, format);
    catch  err
        error(err.message);
    end
end