function tixiCreateElementAtIndex(handle, xpath, name, index)
    if (ischar(handle))
        error('Invalid handle');
    end
    
    if not(ischar(xpath))
        error('Invalid xpath');
    end
    
    if not(ischar(name))
        error('Invalid xpath');
    end
    
    if ischar(index)
        error('Invalid index');
    end
    
    try 
        tixi_matlab('tixiCreateElementAtIndex',handle, xpath, name, index);
    catch  err
        error(err.message);
    end
end