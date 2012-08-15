function tixiAddPoint(handle, parentPath, vPoint, format)
    if (ischar(handle))
        error('Invalid handle');
    end
    
    if not(ischar(parentPath))
        error('Invalid parent path');
    end
    
    if not(ischar(format))
        error('Invalid format argument');
    end
    
    if numel(vPoint) ~= 3
        error('Dimension of point argument has to be 3.');
    end
    
    try 
        tixi_matlab('tixiAddPoint',handle, parentPath, vPoint, format);
    catch  err
        error(err.message);
    end
end