function tixiCloseAllDocuments()
    try
        tixi_matlab('tixiCloseAllDocuments');
    catch err
        error(err.message);
    end
end