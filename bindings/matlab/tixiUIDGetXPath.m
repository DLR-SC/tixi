function xpath = tixiUIDGetXPath(handle, uid)
    if (ischar(handle))
        error('Invalid handle');
    end
    
    if not(ischar(uid))
        error('Invalid UID');
    end
    
    try 
        xpath = tixi_matlab('tixiUIDGetXPath',handle, uid);
    catch  err
        error(err.message);
    end
end