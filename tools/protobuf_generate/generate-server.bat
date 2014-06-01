@echo off

rem 处理服务器协议
cd .\ProtoServer\
mkdir .\out
..\protoc *.proto --cpp_out=./out
xcopy /y .\out\*.* ..\..\..\libgame\ProtoServer\
rmdir /Q/S .\out\
cd ..

pause