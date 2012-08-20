function handle = tixiOpenDocumentFromHTTP(url)
    if(nargin~=1)
        error('Please specify a url');
    end
    
    if not(ischar(url))
        error('Invalid url argument');
    end
    try
        handle = tixi_matlab('tixiOpenDocumentFromHTTP',url);
    catch err
        error(err.message);
    end
end