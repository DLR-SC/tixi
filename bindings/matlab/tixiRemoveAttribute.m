function tixiRemoveAttribute(handle, xpath, attname)
    if (ischar(handle))
        error('Invalid handle');
    end
    
    if not(ischar(xpath))
        error('Invalid xpath');
    end
   
        if not(ischar(xpath))
        error('Invalid attribute name');
    end
    
    try 
        tixi_matlab('tixiRemoveAttribute',handle, xpath, attname);
    catch  err
        error(err.message);
    end
end