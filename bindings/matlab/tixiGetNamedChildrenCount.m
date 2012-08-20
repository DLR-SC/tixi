function val = tixiGetNamedChildrenCount(handle, parentPath, childName)
    if (ischar(handle))
        error('Invalid handle');
    end
    
    if not(ischar(parentPath))
        error('Invalid parent path');
    end
    
    if not(ischar(childName))
        error('Invalid child name');
    end   
    
    try 
        val = tixi_matlab('tixiGetNamedChildrenCount',handle, parentPath, childName);
    catch  err
        error(err.message);
    end
end