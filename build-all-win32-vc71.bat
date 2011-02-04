@echo on

set SOLUTION_NAME="Src\TIXI.sln"

REM Build static library version
devenv /clean Release %SOLUTION_NAME%
devenv /build Release %SOLUTION_NAME%

REM Build dll library version
devenv /clean Release_Dll /project TIXI %SOLUTION_NAME%
devenv /build Release_Dll /project TIXI %SOLUTION_NAME%

REM Build static library version
devenv /clean Release /project TIXI %SOLUTION_NAME%
devenv /build Release /project TIXI %SOLUTION_NAME%

rem - Build MinGW version
cd Src
make -f Makefile.windows clean
make -f Makefile.windows

REM create python wrapper code
python make-wrapper.py

REM Build API-Docu
cd ..
rm -rf Doc/html
doxygen Doc/Doxyfile

rem - Build structure
rm -rf Install
mkdir Install\Static
mkdir Install\Dynamic
mkdir Install\Include
mkdir Install\MinGW-Version
mkdir Install\Doc

copy /Y Src\Release\TIXI.lib Install\Static
copy /Y Src\Release_Dll\TIXI.lib Install\Dynamic
copy /Y Src\Release_Dll\TIXI.dll Install\Dynamic
copy /Y Src\tixi.h Install\Include
copy /Y lib\*.dll Install\MinGW-Version
copy /Y lib\lib* Install\MinGW-Version
xcopy /E /Y doc\html Install\doc
copy Src\wrapper.py Install\tixiwrapper.py

rem - zip it 
7za a tixi-0.X_win32_vc7_static_shared.zip Install


rem - Build Source Distribution
rm -rf tixi-src
svn export http://tixi.googlecode.com/svn/trunk/ tixi-src
7za a tixi-0.X_src.zip tixi-src 

#scons build
pack\pack_xp.bat

