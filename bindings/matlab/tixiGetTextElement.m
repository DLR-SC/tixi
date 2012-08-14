function text = tixiGetTextElement(handle, xpath)
    if (ischar(handle))
        error('Invalid handle');
    end
    
    if not(ischar(xpath))
        error('Invalid xpath');
    end
    
    try 
        text = tixi_matlab('tixiGetTextElement',handle, xpath);
    catch  err
        error(err.message);
    end
end