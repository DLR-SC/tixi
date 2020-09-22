function found = tixiUIDCheckExists(handle, uID)
    if (ischar(handle))
        error('Invalid type of argument "handle"');
    end

    if not(ischar(uID))
        error('Invalid type of argument "uID"');
    end

    try
        tixi3_matlab('tixiUIDCheckExists', handle, uID);
        found = true;
    catch err
        if strfind(err.message, 'returned code 32') || strfind(err.message, 'returned code 33')
            found = false;
        else
            error(err.message);
        end
    end
end

