function tixiSchemaValidateFromString(handle, string)
    if (ischar(handle))
        error('Invalid handle');
    end
    
    if not(ischar(string))
        error('Invalid string');
    end
    
    try 
        tixi_matlab('tixiSchemaValidateFromString',handle, string);
    catch  err
        error(err.message);
    end
end