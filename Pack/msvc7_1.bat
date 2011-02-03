set FLAGS="/Od  /D WIN32 /D NDEBUG /D _LIB /D FORTRAN /D IFORT_WIN32 /D _MBCS /MLd /Za  /c  /TC"
set LIBXML2_HOME=C:\DLR\TIVA\local
set MSVCVersion=7.1
set RT=ML
(scons Pack=yes Tool=msvc MSVSVersion=%MSVCVersion% FC=ifort -c . & scons Pack=yes  Tool=msvc MSVSVersion=%MSVCVersion%  FC=ifort CCFLAGS=%FLAGS% ) & ( del tixi_win32_msvc7.1_ifort_%RT%.zip & ren tixi_win32_msvc7.1_ifort.zip tixi_win32_msvc7.1_ifort_%RT%.zip)
