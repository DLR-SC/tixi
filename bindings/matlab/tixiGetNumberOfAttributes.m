function val = tixiGetNumberOfAttributes(handle, parentPath)
    if (ischar(handle))
        error('Invalid handle');
    end
    
    if not(ischar(parentPath))
        error('Invalid parent path');
    end
    
    try 
        val = tixi_matlab('tixiGetNumberOfAttributes',handle, parentPath);
    catch  err
        error(err.message);
    end
end