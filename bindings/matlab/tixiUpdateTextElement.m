function tixiUpdateTextElement(handle, xpath, text)
    if (ischar(handle))
        error('Invalid handle');
    end
    
    if not(ischar(xpath))
        error('Invalid xpath');
    end
    
    if not(ischar(text))
        error('Invalid text argument');
    end
    
    try 
        tixi_matlab('tixiUpdateTextElement',handle, xpath, text);
    catch  err
        error(err.message);
    end
end