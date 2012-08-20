function val = tixiCheckElement(handle, elementName)
    if (ischar(handle))
        error('Invalid handle');
    end
    
    if ~ischar(elementName)
        error('Invalid element name');
    end
    
    try 
        val = tixi_matlab('tixiCheckElement',handle, elementName);
    catch  err
        error(err.message);
    end
end