function string = tixiExportDocumentAsString(handle)
    if(nargin~=1)
        error('Please specify a handle');
    end
    
    if (ischar(handle))
        error('Invalid handle');
    end
    try 
        string = tixi_matlab('tixiExportDocumentAsString',handle);
    catch  err
        error(err.message);
    end
end