@echo off

rem ����ͨ��Э��
cd .\ProtoCommon\
mkdir .\out
..\protoc ProtoGameData.proto --cpp_out=./out
xcopy /y .\out\ProtoGameData.* ..\..\..\libgame\ProtoCommon\
rmdir /Q/S .\out\
cd ..

pause