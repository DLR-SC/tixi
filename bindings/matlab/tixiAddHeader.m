function tixiAddHeader(handle, toolName, version, authorName)
    if (ischar(handle))
        error('Invalid handle');
    end
    
    if not(ischar(toolName))
        error('Invalid tool name');
    end
    
    if not(ischar(version))
        error('Invalid version. Version must be a string.');
    end
    
    if not(ischar(authorName))
        error('Invalid author name');
    end
    
    try 
        tixi_matlab('tixiAddHeader',handle, toolName, version, authorName);
    catch  err
        error(err.message);
    end
end