function tixiAddTextElement(handle, xpath, name, text)
    if (ischar(handle))
        error('Invalid handle');
    end
    
    if not(ischar(xpath))
        error('Invalid xpath');
    end
    
    if not(ischar(name))
        error('Invalid xpath');
    end
    
    if not(ischar(text))
        error('Invalid xpath');
    end
    
    try 
        tixi_matlab('tixiAddTextElement',handle, xpath, name, text);
    catch  err
        error(err.message);
    end
end