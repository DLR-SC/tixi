function tixiAddCpacsHeader(handle, name, creator, version, description, cpacsVersion)
    if (ischar(handle))
        error('Invalid handle');
    end
    
    if not(ischar(name))
        error('Invalid name');
    end
    
    if not(ischar(version))
        error('Invalid version. Version must be a string.');
    end
    
    if not(ischar(creator))
        error('Invalid creator argument');
    end
    
    if not(ischar(description))
        error('Invalid description argument');
    end
    
    if not(ischar(cpacsVersion))
        error('Invalid cpacs version argument');
    end
    
    try 
        tixi_matlab('tixiAddCpacsHeader',handle, name, creator, version, description, cpacsVersion);
    catch  err
        error(err.message);
    end
end