@echo off

set SOLUTION_NAME="TIXI.sln"

cd ..\SRC
REM Build static library version
devenv /clean Release %SOLUTION_NAME%
devenv /build Release %SOLUTION_NAME%

REM Build dll library version
devenv /clean Release_Dll /project TIXI %SOLUTION_NAME%
devenv /build Release_Dll /project TIXI %SOLUTION_NAME%

mkdir ..\Install\Static
mkdir ..\Install\Dynamic
mkdir ..\Install\Include

copy /Y Release\TIXI.lib ..\Install\Static
copy /Y Release_Dll\TIXI.lib ..\Install\Dynamic
copy /Y Release_Dll\TIXI.dll ..\Install\Dynamic
copy /Y tixi.h ..\Install\Include
