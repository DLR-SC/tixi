function tixiUIDCheckLinks(handle)
    if(nargin~=1)
        error('Please specify a handle');
    end
    
    if (ischar(handle))
        error('Invalid handle');
    end
    try 
        tixi_matlab('tixiUIDCheckLinks',handle);
    catch  err
        error(err.message);
    end
end