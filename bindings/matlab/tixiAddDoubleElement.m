function tixiAddDoubleElement(handle, xpath, name, dvalue, format)
    if (ischar(handle))
        error('Invalid handle');
    end
    
    if not(ischar(xpath))
        error('Invalid xpath');
    end
    
    if not(ischar(name))
        error('Invalid tag name');
    end
    
    if ischar(dvalue)
        error('Invalid value');
    end
    
    if not(ischar(format))
        error('Invalid format argument');
    end
    
    try 
        tixi_matlab('tixiAddDoubleElement',handle, xpath, name, dvalue, format);
    catch  err
        error(err.message);
    end
end