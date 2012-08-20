function tixiAddExternalLink(handle, xpath, url, fileformat)
    if (ischar(handle))
        error('Invalid handle');
    end
    
    if not(ischar(xpath))
        error('Invalid xpath');
    end
    
    if not(ischar(url))
        error('Invalid attribute name');
    end
    
    if not(ischar(fileformat))
        error('Invalid attribute value');
    end
    
    try 
        tixi_matlab('tixiAddExternalLink',handle, xpath, url, fileformat);
    catch  err
        error(err.message);
    end
end