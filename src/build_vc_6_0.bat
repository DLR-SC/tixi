set FLAGS="/Od  /D WIN32  /D _LIB  /D _MBCS /MD /Za  /c  /TC /Gd "
set LIBXML2_HOME=c:\Programme\libxml2
set CUNIT_HOME=C:\Programme\Cunit\2_0_2
set MSVCVersion=6.0
scons  Tool=msvc MSVSVersion=%MSVCVersion%  CCFLAGS=%FLAGS%  %*
