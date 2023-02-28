@echo off

pushd %~dp0\
call %ATOM_DIR%\Vendor\premake\bin\premake5.exe vs2022
popd
