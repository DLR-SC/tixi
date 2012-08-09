set -x
export CCFLAGS="-g3 -Wall -ansi -pedantic"
export FCFLAGS="-g3"
export LIBXML2_HOME=/sc
export CUNIT_HOME=/sc
cd /DLR/TIVA/TIXI
scons $*
