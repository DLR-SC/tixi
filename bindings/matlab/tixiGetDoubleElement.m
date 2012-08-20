function val = tixiGetDoubleElement(handle, xpath)
    if (ischar(handle))
        error('Invalid handle');
    end
    
    if not(ischar(xpath))
        error('Invalid xpath');
    end
    
    try 
        val = tixi_matlab('tixiGetDoubleElement',handle, xpath);
    catch  err
        error(err.message);
    end
end