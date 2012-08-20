function val =  tixiXPathEvaluateNodeNumber(handle, xPathExpression)
    if (ischar(handle))
        error('Invalid handle');
    end
    
    if not(ischar(xPathExpression))
        error('Invalid xpath');
    end
    
    try 
        val = tixi_matlab('tixiXPathEvaluateNodeNumber',handle, xPathExpression);
    catch  err
        error(err.message);
    end
end