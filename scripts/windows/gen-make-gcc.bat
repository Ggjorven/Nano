@echo off

pushd %~dp0\..\..\

call premake5 gmake --os=windows --cc=gcc

popd
PAUSE
