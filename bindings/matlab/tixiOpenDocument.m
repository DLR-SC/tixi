function handle = tixiOpenDocument(filename)
    if(nargin~=1)
        error('Please specify a filename');
    end
    
    if not(ischar(filename))
        error('Invalid filename argument');
    end
    handle = tixi_matlab('tixiOpenDocument',filename);
end