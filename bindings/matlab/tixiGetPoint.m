function point = tixiGetPoint(handle, path)
    if (ischar(handle))
        error('Invalid handle');
    end
    
    if not(ischar(path))
        error('Invalid path');
    end
    
    try 
        point = tixi_matlab('tixiGetPoint',handle, path);
    catch  err
        error(err.message);
    end
end