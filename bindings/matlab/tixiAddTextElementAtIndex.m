function tixiAddTextElementAtIndex(handle, xpath, name, text, index)
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
    
    if ischar(index)
        error('Invalid index');
    end
    
    try 
        tixi_matlab('tixiAddTextElementAtIndex',handle, xpath, name, text, index);
    catch  err
        error(err.message);
    end
end