function handle = tixiImportFromString(string)
    if(nargin~=1)
        error('Please specify a string');
    end
    
    if not(ischar(string))
        error('Invalid string argument');
    end
    try
        handle = tixi_matlab('tixiImportFromString',string);
    catch err
        error(err.message);
    end
end