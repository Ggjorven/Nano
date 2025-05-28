@echo off

pushd %~dp0\..\..\

call premake5 vs2022 --os=windows

popd
PAUSE