function val = tixiCheckDocumentHandle(handle)
    if (ischar(handle))
        error('Invalid handle');
    end
    
    try 
        val = tixi_matlab('tixiCheckDocumentHandle',handle);
    catch  err
        error(err.message);
    end
end