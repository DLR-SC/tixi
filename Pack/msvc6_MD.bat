set FLAGS="/Od  /D WIN32 /D NDEBUG /D _LIB /D FORTRAN /D IFORT_WIN32 /D _MBCS /MDd /Za  /c  /TC"
set LIBXML2_HOME=c:\Programme\libxml2
set MSVCVersion=6.0
set RT=MD
(scons Pack=yes Tool=msvc MSVSVersion=%MSVCVersion% FC=ifort -c . & scons Pack=yes  Tool=msvc MSVSVersion=%MSVCVersion% FC=ifort CCFLAGS=%FLAGS%) & (   del tixi_win32_msvc%MSVCVersion%_ifort_%RT%.zip &   rename tixi_win32_msvc%MSVCVersion%_ifort.zip   tixi_win32_msvc%MSVCVersion%_ifort_%RT%.zip)
