function tixiAddFloatVector(handle, xpath, name, vector)
    if (ischar(handle))
        error('Invalid handle');
    end
    
    if not(ischar(xpath))
        error('Invalid xpath');
    end
    
    if not(ischar(name))
        error('Invalid vector name');
    end
    
    if (ischar(vector))
        error('Invalid vector');
    end
    
    try 
        tixi_matlab('tixiAddFloatVector',handle, xpath, name, vector);
    catch  err
        error(err.message);
    end
end