function isvalid = tixiCheckDocumentHandle(handle)
    if (ischar(handle))
        error('Invalid type of argument "handle"');
    end

    try
        tixi3_matlab('tixiCheckDocumentHandle', handle);
        isvalid = true;
    catch err
        if strfind(err.message, 'returned code 6')
            isvalid = false;
        else
            error(err.message);
        end
    end
end

