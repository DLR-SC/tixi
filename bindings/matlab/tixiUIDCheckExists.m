function exists = tixiUIDCheckExists(handle, uid)
    if (ischar(handle))
        error('Invalid handle');
    end
    
    if not(ischar(uid))
        error('Invalid UID');
    end
    
    try 
        exists = tixi_matlab('tixiUIDCheckExists',handle, uid);
    catch  err
        error(err.message);
    end
end