function tixiSchemaValidateFromFile(handle, filename)
    if (ischar(handle))
        error('Invalid handle');
    end
    
    if not(ischar(filename))
        error('Invalid file name');
    end
    
    try 
        tixi_matlab('tixiSchemaValidateFromFile',handle, filename);
    catch  err
        error(err.message);
    end
end