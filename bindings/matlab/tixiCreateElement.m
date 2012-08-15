function tixiCreateElement(handle, xpath, name)
    if (ischar(handle))
        error('Invalid handle');
    end
    
    if not(ischar(xpath))
        error('Invalid xpath');
    end
    
    if not(ischar(name))
        error('Invalid xpath');
    end
    
    try 
        tixi_matlab('tixiCreateElement',handle, xpath, name);
    catch  err
        error(err.message);
    end
end