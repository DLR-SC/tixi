function handle = tixiOpenDocumentRecursive(filename, openMode)
    if(nargin~=2)
        error('Please specify a filename and the openmode.');
    end
    
    
    if not(ischar(filename))
        error('Invalid filename');
    end
    
    mode = -1;
    
    if ischar(openMode)
        if strcmp(openMode, 'plain') > 0
            mode = 0;
        elseif strcmp(openMode, 'recursive') > 0
            mode = 1;
        end
    else
        if openMode == 0
            mode = 0;
        elseif openMode == 1;
            mode = 1;
        end
    end
    
    if mode == -1
        error('Invalid openmode. Valid options are 0,1, plain and recursive.');
    end
    
    try 
        handle = tixi_matlab('tixiOpenDocumentRecursive', filename, mode);
    catch  err
        error(err.message);
    end
end