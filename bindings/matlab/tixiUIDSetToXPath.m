function tixiUIDSetToXPath(handle, xpath, uid)
    if (ischar(handle))
        error('Invalid handle');
    end
    
    if not(ischar(xpath))
        error('Invalid xpath argument');
    end
    
    if not(ischar(uid))
        error('Invalid UID');
    end
    
    try 
        tixi_matlab('tixiUIDSetToXPath',handle, xpath, uid);
    catch  err
        error(err.message);
    end
end