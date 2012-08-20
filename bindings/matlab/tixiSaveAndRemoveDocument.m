function tixiSaveAndRemoveDocument(handle, filename)
    if(nargin~=2)
        error('Please specify a handle and a filename');
    end
    
    if (ischar(handle))
        error('Invalid handle');
    end
    
    if not(ischar(filename))
        error('Invalid filename');
    end
    
    try 
        tixi_matlab('tixiSaveAndRemoveDocument',handle, filename);
    catch  err
        error(err.message);
    end
end