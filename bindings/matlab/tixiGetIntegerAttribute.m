function value = tixiGetIntegerAttribute(handle, path, attributeName)
    if (ischar(handle))
        error('Invalid handle');
    end
    
    if not(ischar(path))
        error('Invalid xpath');
    end
    
    if not(ischar(attributeName))
        error('Invalid attribute name');
    end
    
    try 
        value = tixi_matlab('tixiGetIntegerAttribute',handle, path, attributeName);
    catch  err
        error(err.message);
    end
end