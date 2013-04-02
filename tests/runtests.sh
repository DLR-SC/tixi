#!/bin/bash

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../lib/
export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:../lib/

echo "Running unit tests..."
./TIXI-unittests

# now run the python tests
echo $DYLD_LIBRARY_PATH
echo "Running python API tests..."
export PYTHONPATH=$PYTHONPATH:../lib

python wrappertests.py


