function handle = tixiCreateDocument(rootElementName)
    if(nargin~=1)
        error('Please specify the root element name');
    end
    
    if not(ischar(rootElementName))
        error('Invalid root element name argument');
    end
    try
        handle = tixi_matlab('tixiCreateDocument',rootElementName);
    catch err
        error(err.message);
    end
end