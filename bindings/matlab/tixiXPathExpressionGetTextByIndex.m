function val =  tixiXPathExpressionGetTextByIndex(handle, xPathExpression, index)
    if (ischar(handle))
        error('Invalid handle');
    end
    
    if not(ischar(xPathExpression))
        error('Invalid xpath');
    end
    
    if numel(index) ~= 1 || ischar(index)
        error('Invalid index');
    end
    
    try 
        val = tixi_matlab('tixiXPathExpressionGetTextByIndex',handle, xPathExpression, index);
    catch  err
        error(err.message);
    end
end