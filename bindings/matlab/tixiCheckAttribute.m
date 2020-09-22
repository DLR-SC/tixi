function found = tixiCheckAttribute(handle, elementPath, attributeName)
    if (ischar(handle))
        error('Invalid type of argument "handle"');
    end

    if not(ischar(elementPath))
        error('Invalid type of argument "elementPath"');
    end

    if not(ischar(attributeName))
        error('Invalid type of argument "attributeName"');
    end

    try
        tixi3_matlab('tixiCheckAttribute', handle, elementPath, attributeName);
        found = true;
    catch err
        if strfind(err.message, 'returned code 12')
            found = false;
        else
            error(err.message);
        end
    end
end

