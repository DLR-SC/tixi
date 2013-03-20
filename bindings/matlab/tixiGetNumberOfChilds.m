function val = tixiGetNumberOfChilds(handle, parentPath)
    if (ischar(handle))
        error('Invalid handle');
    end
    
    if not(ischar(parentPath))
        error('Invalid parent path');
    end
    
    try 
        val = tixi_matlab('tixiGetNumberOfChilds',handle, parentPath);
    catch  err
        error(err.message);
    end
end