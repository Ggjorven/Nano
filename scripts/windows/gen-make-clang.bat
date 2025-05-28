@echo off

pushd %~dp0\..\..\

call premake5 gmake --os=windows --cc=clang

popd
PAUSE
