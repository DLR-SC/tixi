function tixiUsePrettyPrint(handle, boolean)
    if (ischar(handle))
        error('Invalid handle');
    end
    
    if boolean == 1
        boolean = true;
    elseif boolean == 0
        boolean = false;
    end
    
    if ~islogical(boolean)
        error('Invalid boolean value');
    end
    
    try 
        tixi_matlab('tixiUsePrettyPrint',handle, boolean);
    catch  err
        error(err.message);
    end
end