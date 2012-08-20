function tixiXSLTransformationToFile(handle, xslFilename, resultFilename)
    if (ischar(handle))
        error('Invalid handle');
    end
    
    if not(ischar(xslFilename))
        error('Invalid xpath');
    end
    
    if not(ischar(resultFilename))
        error('Invalid attribute name');
    end
   
    try 
        tixi_matlab('tixiXSLTransformationToFile',handle, xslFilename, resultFilename);
    catch  err
        error(err.message);
    end
end