function found = tixiCheckElement(handle, elementPath)
    if (ischar(handle))
        error('Invalid type of argument "handle"');
    end

    if not(ischar(elementPath))
        error('Invalid type of argument "elementPath"');
    end

    try
        tixi3_matlab('tixiCheckElement', handle, elementPath);
        found = true;
    catch err
        if strfind(err.message, 'returned code 8')
            found = false;
        else
            error(err.message);
        end
    end
end

