function vector = tixiGetFloatVector(handle, xpath, numElem)
    if (ischar(handle))
        error('Invalid handle');
    end
    
    if not(ischar(xpath))
        error('Invalid xpath');
    end
    
    if (ischar(numElem))
        error('Invalid number of elements');
    end
    
    try 
        vector = tixi_matlab('tixiGetFloatVector',handle, xpath, numElem);
    catch  err
        error(err.message);
    end
end