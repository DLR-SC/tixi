function tixiAddTextElement(handle, xpath, name, text)
    if (ischar(handle))
        error('Invalid handle');
    end
    
    if not(ischar(xpath))
        error('Invalid xpath');
    end
    
    if not(ischar(name))
        error('Invalid tag name');
    end
    
    if not(ischar(text))
        error('Invalid text');
    end
    
    try 
        tixi_matlab('tixiAddTextElement',handle, xpath, name, text);
    catch  err
        error(err.message);
    end
end