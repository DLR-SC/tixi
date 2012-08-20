function handle = tixiOpenDocument(filename)
    if(nargin~=1)
        error('Please specify a filename');
    end
    
    if not(ischar(filename))
        error('Invalid filename argument');
    end
    try
        handle = tixi_matlab('tixiOpenDocument',filename);
    catch err
        error(err.message);
    end
end