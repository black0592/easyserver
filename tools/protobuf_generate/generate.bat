@echo off

rem 处理通用协议
cd .\ProtoCommon\
mkdir .\out
..\protoc *.proto --cpp_out=./out
xcopy /y .\out\*.* ..\..\..\libgame\ProtoCommon\ /EXCLUDE:exclude.txt
rmdir /Q/S .\out\
cd ..

pause