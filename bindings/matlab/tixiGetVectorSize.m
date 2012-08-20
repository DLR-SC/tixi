function size = tixiGetVectorSize(handle, xpath)
    if (ischar(handle))
        error('Invalid handle');
    end
    
    if not(ischar(xpath))
        error('Invalid xpath');
    end
    
    try 
        size = tixi_matlab('tixiGetVectorSize',handle, xpath);
    catch  err
        error(err.message);
    end
end