
set RT=crtdll
set GCC_VERSION=2.95.2
set LIBXML2_HOME=c:\libxml-gcc-%GCC_VERSION%_%RT%
set GCC_BIN=c:\gcc-%GCC_VERSION%_%RT%\bin

scons Pack=yes Tool=mingw FC=g77 GccBinDir=%GCC_BIN% RuntimeLib=%RT% -c . & scons Pack=yes Tool=mingw FC=g77 GccBinDir=%GCC_BIN% RuntimeLib=%RT%


