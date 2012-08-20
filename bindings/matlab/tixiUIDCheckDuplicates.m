function val = tixiUIDCheckDuplicates(handle)
    if(nargin~=1)
        error('Please specify a handle');
    end
    
    if (ischar(handle))
        error('Invalid handle');
    end
    try 
        val = tixi_matlab('tixiUIDCheckDuplicates',handle);
    catch  err
        error(err.message);
    end
end