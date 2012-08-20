function tixiCloseDocument(handle)
    if(nargin~=1)
        error('Please specify a handle');
    end
    
    if (ischar(handle))
        error('Invalid handle');
    end
    try 
        tixi_matlab('tixiCloseDocument',handle);
    catch  err
        error(err.message);
    end
end