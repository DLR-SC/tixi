function tixiDTDValidate(handle, DTDFilename)
    if (ischar(handle))
        error('Invalid handle');
    end
    
    if not(ischar(DTDFilename))
        error('Invalid DTDFilename');
    end
    
    try 
        tixi_matlab('tixiDTDValidate',handle, DTDFilename);
    catch  err
        error(err.message);
    end
end