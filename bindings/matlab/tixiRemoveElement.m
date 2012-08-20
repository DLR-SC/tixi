function tixiRemoveElement(handle, xpath)
    if (ischar(handle))
        error('Invalid handle');
    end
    
    if not(ischar(xpath))
        error('Invalid xpath');
    end
   
    try 
        tixi_matlab('tixiRemoveElement',handle, xpath);
    catch  err
        error(err.message);
    end
end