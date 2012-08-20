function val = tixiCheckAttribute(handle, xpath, name)
    if (ischar(handle))
        error('Invalid handle');
    end
    
    if ~ischar(xpath)
        error('Invalid xpath');
    end
    
    if ~ischar(name)
        error('Invalid attribute name');
    end
    
    try 
        val = tixi_matlab('tixiCheckAttribute',handle, xpath, name);
    catch  err
        error(err.message);
    end
end