function tixiAddBooleanElement(handle, xpath, name, boolean)
    if (ischar(handle))
        error('Invalid handle');
    end
    
    if not(ischar(xpath))
        error('Invalid xpath');
    end
    
    if not(ischar(name))
        error('Invalid tag name');
    end
    
    if boolean == 1
        boolean = true;
    elseif boolean == 0
        boolean = false;
    end
    
    if ~islogical(boolean)
        error('Invalid boolean value');
    end
    
    try 
        tixi_matlab('tixiAddBooleanElement',handle, xpath, name, boolean);
    catch  err
        error(err.message);
    end
end