@echo off

rem 处理通用协议
cd .\ProtoCommon\
mkdir .\out
..\protoc ProtoGameData.proto --cpp_out=./out
xcopy /y .\out\ProtoGameData.* ..\..\..\libgame\ProtoCommon\
rmdir /Q/S .\out\
cd ..

pause