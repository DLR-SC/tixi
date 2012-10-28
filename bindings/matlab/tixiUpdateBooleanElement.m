function tixiUpdateBooleanElement(handle, xpath, boolean)
    if (ischar(handle))
        error('Invalid handle');
    end
    
    if not(ischar(xpath))
        error('Invalid xpath');
    end
    
    if (ischar(boolean))
        error('Invalid boolean argument');
    end
    
    try 
        tixi_matlab('tixiUpdateBooleanElement',handle, xpath, boolean);
    catch  err
        error(err.message);
    end
end